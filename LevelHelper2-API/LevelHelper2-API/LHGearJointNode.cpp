//
//  LHGearJointNode.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 27/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHGearJointNode.h"
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

LHGearJointNode::LHGearJointNode()
{
    _jointA = nullptr;
    _jointB = nullptr;
}

LHGearJointNode::~LHGearJointNode()
{
    this->shouldRemoveJoint();
    _jointA = nullptr;
    _jointB = nullptr;
}

LHGearJointNode* LHGearJointNode::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHGearJointNode *ret = new LHGearJointNode();
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
bool LHGearJointNode::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        
        this->loadGenericInfoFromDictionary(dict);
        this->loadTransformationInfoFromDictionary(dict);

        prnt->addChild(this);

        this->loadJointInfoFromDictionary(dict);
        
        _jointAUUID = dict->stringForKey("jointAUUID");
        _jointBUUID = dict->stringForKey("jointBUUID");
        _ratio = dict->floatForKey("gearRatio");
        
        return true;
    }
    return false;
}

void LHGearJointNode::findConnectedJoints()
{
    if(_jointAUUID.length() == 0 || _jointBUUID.length() == 0)
        return;
    
    Node* node = dynamic_cast<Node*>(this);
    if(!node)return;
    
    LHScene* scene = (LHScene*)node->getScene();
    
    LHNodeProtocol* parentProtocol = dynamic_cast<LHNodeProtocol*>(node->getParent());
    if(parentProtocol)
    {
        _jointA = parentProtocol->getChildNodeWithUUID(_jointAUUID);
        _jointB = parentProtocol->getChildNodeWithUUID(_jointBUUID);
    }
    else{
        _jointA = scene->getChildNodeWithUUID(_jointAUUID);
        _jointB = scene->getChildNodeWithUUID(_jointBUUID);
    }
}


#if COCOS2D_VERSION >= 0x00030200
void LHGearJointNode::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
#else
void LHGearJointNode::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
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

bool LHGearJointNode::lateLoading()
{
    this->findConnectedNodes();
    
    this->findConnectedJoints();
    
    Node* nodeA = this->getNodeA();
    Node* nodeB = this->getNodeB();
    
    if(nodeA && nodeB && _jointA && _jointB)
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
        
        LHJointsProtocol* jointAProt = dynamic_cast<LHJointsProtocol*>(_jointA);
        LHJointsProtocol* jointBProt = dynamic_cast<LHJointsProtocol*>(_jointB);
        
        b2Joint* jtA = jointAProt->getJoint();
        b2Joint* jtB = jointBProt->getJoint();
        
        b2GearJointDef jointDef;
        
        jointDef.joint1 = jtA;
        jointDef.joint2 = jtB;
        jointDef.bodyA = bodyA;
        jointDef.bodyB = bodyB;
        jointDef.ratio = _ratio;
        
        jointDef.collideConnected = this->getCollideConnected();
        
        b2GearJoint* joint = (b2GearJoint*)world->CreateJoint(&jointDef);
        
        joint->SetUserData(this);
        this->setJoint(joint);
                
#else//chipmunk
        if(nodeA->getPhysicsBody() && nodeB->getPhysicsBody())
        {
            CCLOG("\n\nWARNING: Gear joint is not supported when using Chipmunk physics engine.\n\n");
            
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
