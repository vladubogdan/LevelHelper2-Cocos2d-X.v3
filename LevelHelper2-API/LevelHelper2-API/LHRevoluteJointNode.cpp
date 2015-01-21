//
//  LHRevoluteJointNode.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 27/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHRevoluteJointNode.h"
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

LHRevoluteJointNode::LHRevoluteJointNode()
{
    
}

LHRevoluteJointNode::~LHRevoluteJointNode()
{
    this->shouldRemoveJoint();
}

LHRevoluteJointNode* LHRevoluteJointNode::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHRevoluteJointNode *ret = new LHRevoluteJointNode();
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
bool LHRevoluteJointNode::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        
        this->loadGenericInfoFromDictionary(dict);
        this->loadTransformationInfoFromDictionary(dict);

        prnt->addChild(this);

        this->loadJointInfoFromDictionary(dict);
        
        
        _enableLimit = dict->boolForKey("enableLimit");
        _enableMotor = dict->boolForKey("enableMotor");
        
        
        _lowerAngle = CC_DEGREES_TO_RADIANS(-dict->floatForKey("lowerAngle"));
        _upperAngle = CC_DEGREES_TO_RADIANS(-dict->floatForKey("upperAngle"));
        
        _maxMotorTorque = dict->floatForKey("maxMotorTorque");
        _motorSpeed     = CC_DEGREES_TO_RADIANS(-360.0*dict->floatForKey("motorSpeed"));
        
        return true;
    }
    return false;
}

#if COCOS2D_VERSION >= 0x00030200
void LHRevoluteJointNode::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
#else
void LHRevoluteJointNode::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
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

bool LHRevoluteJointNode::lateLoading()
{
    this->findConnectedNodes();
    
    Node* nodeA = this->getNodeA();
    Node* nodeB = this->getNodeB();
    
    Point relativePosA = this->getLocalAnchorA();

    
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
        
        b2RevoluteJointDef jointDef;
        
        jointDef.Initialize(bodyA,
                            bodyB,
                            posA);
        
        jointDef.collideConnected = this->getCollideConnected();
        
        jointDef.enableLimit = _enableLimit;
        jointDef.enableMotor = _enableMotor;
        
        if(_lowerAngle < _upperAngle){
            jointDef.lowerAngle = _lowerAngle;
            jointDef.upperAngle = _upperAngle;
        }
        else{
            jointDef.lowerAngle = _upperAngle;
            jointDef.upperAngle = _lowerAngle;
        }
        
        jointDef.maxMotorTorque = _maxMotorTorque;
        jointDef.motorSpeed     = _motorSpeed;
        
        b2RevoluteJoint* joint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
        joint->SetUserData(this);
        
        this->setJoint(joint);
        
#else//chipmunk
        if(nodeA->getPhysicsBody() && nodeB->getPhysicsBody())
        {
            CCLOG("\n\nWARNING: Revolute joint is not supported when using Chipmunk physics engine.\n\n");
            
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
