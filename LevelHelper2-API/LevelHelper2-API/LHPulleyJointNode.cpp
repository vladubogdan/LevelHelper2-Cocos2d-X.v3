//
//  LHPulleyJointNode.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 27/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHPulleyJointNode.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHDevice.h"
#include "LHUtils.h"
#include "LHDrawNode.h"
#include "LHValue.h"
#include "LHGameWorldNode.h"
#include "LHPhysicsProtocol.h"


#if LH_USE_BOX2D
#include "Box2d/Box2d.h"
#endif

LHPulleyJointNode::LHPulleyJointNode()
{
    
}

LHPulleyJointNode::~LHPulleyJointNode()
{
    this->shouldRemoveJoint();
}

LHPulleyJointNode* LHPulleyJointNode::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHPulleyJointNode *ret = new LHPulleyJointNode();
    if (ret && ret->initWithDictionary(dict, prnt))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}
bool LHPulleyJointNode::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        
        this->loadGenericInfoFromDictionary(dict);
        this->loadTransformationInfoFromDictionary(dict);

        prnt->addChild(this);

        this->loadJointInfoFromDictionary(dict);
        
        LHScene* scene    = (LHScene*)prnt->getScene();
        Size designSize   = scene->getDesignResolutionSize();
        Point offset      = scene->getDesignOffset();
        
        _groundAnchorA = dict->pointForKey("groundAnchorA");
        {
            _groundAnchorA = Point(_groundAnchorA.x, designSize.height - _groundAnchorA.y);
            _groundAnchorA.x += offset.x;
            _groundAnchorA.y += offset.y;
        }
        
        _groundAnchorB = dict->pointForKey("groundAnchorB");
        {
            _groundAnchorB = Point(_groundAnchorB.x, designSize.height - _groundAnchorB.y);
            _groundAnchorB.x += offset.x;
            _groundAnchorB.y += offset.y;
        }
        
        _ratio = dict->floatForKey("ratio");
        
        return true;
    }
    return false;
}

#if COCOS2D_VERSION >= 0x00030200
void LHPulleyJointNode::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
#else
void LHPulleyJointNode::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
#endif
{
    if(!this->getNodeA() ||  !this->getNodeB()){
        this->lateLoading();
    }
    
    if(renderer)
    {
#if COCOS2D_VERSION >= 0x00030200
        Node::visit(renderer, parentTransform, parentFlags);
#else
        Node::visit(renderer, parentTransform, parentTransformUpdated);
#endif
    }
}

bool LHPulleyJointNode::lateLoading()
{
    this->findConnectedNodes();
    
    Node* nodeA = this->getNodeA();
    Node* nodeB = this->getNodeB();
    
    Point relativePosA = this->getLocalAnchorA();
    Point relativePosB = this->getLocalAnchorB();
    
    if(nodeA && nodeB)
    {
        //this ensures that all transformations are set on the body prior creating the joint
        nodeA->setPosition(nodeA->getPosition());
        nodeB->setPosition(nodeB->getPosition());

#if LH_USE_BOX2D
        LHScene* scene = (LHScene*)this->getScene();
        LHGameWorldNode* pNode = scene->getGameWorldNode();
        b2World* world = pNode->getBox2dWorld();
        if(world == nullptr)return false;
        
        b2Body* bodyA = LH_GET_BOX2D_BODY(this->getNodeA());
        b2Body* bodyB = LH_GET_BOX2D_BODY(this->getNodeB());
        
        if(!bodyA || !bodyB)return false;
        
        b2Vec2 relativeA = scene->metersFromPoint(relativePosA);
        b2Vec2 posA = bodyA->GetWorldPoint(relativeA);

        b2Vec2 relativeB = scene->metersFromPoint(relativePosB);
        b2Vec2 posB = bodyB->GetWorldPoint(relativeB);
        
        b2PulleyJointDef jointDef;
        
        b2Vec2 grdA = scene->metersFromPoint(_groundAnchorA);
        b2Vec2 grdB = scene->metersFromPoint(_groundAnchorB);
        
        jointDef.Initialize(bodyA,
                            bodyB,
                            grdA,
                            grdB,
                            posA,
                            posB,
                            _ratio);
        
        jointDef.collideConnected = this->getCollideConnected();
        
        b2PulleyJoint* joint = (b2PulleyJoint*)world->CreateJoint(&jointDef);
        joint->SetUserData(this);
        this->setJoint(joint);
        
#else//chipmunk
        if(nodeA->getPhysicsBody() && nodeB->getPhysicsBody())
        {
            CCLOG("\n\nWARNING: Pulley joint is not supported when using Chipmunk physics engine.\n\n");
            
//            PhysicsJointLimit* joint = PhysicsJointLimit::construct(nodeA->getPhysicsBody(),
//                                                                    nodeB->getPhysicsBody(),
//                                                                    relativePosA,
//                                                                    relativePosB,
//                                                                    0,
//                                                                    _length);
//
//            this->getScene()->getPhysicsWorld()->addJoint(joint);
//
//            this->setJoint(joint);
            
            return true;
        }
#endif
        
        
    }

    return false;
}
