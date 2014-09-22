//
//  LHWheelJointNode.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 27/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHWheelJointNode.h"
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

LHWheelJointNode::LHWheelJointNode()
{
    
}

LHWheelJointNode::~LHWheelJointNode()
{
    this->shouldRemoveJoint();
}

LHWheelJointNode* LHWheelJointNode::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHWheelJointNode *ret = new LHWheelJointNode();
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
bool LHWheelJointNode::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        
        this->loadGenericInfoFromDictionary(dict);
        this->loadTransformationInfoFromDictionary(dict);

        prnt->addChild(this);

        this->loadJointInfoFromDictionary(dict);
        
        _enableMotor = dict->boolForKey("enableWheelMotor");
        
        _frequency  = dict->floatForKey("wheelFrequencyHz");
        _damping    = dict->floatForKey("wheelDampingRatio");
        
        _maxMotorTorque = dict->floatForKey("wheelMaxMotorForce");
        _motorSpeed = dict->floatForKey("wheelMotorSpeed");
        
        _axis = dict->pointForKey("axis");
        
        return true;
    }
    return false;
}

#if COCOS2D_VERSION >= 0x00030200
void LHWheelJointNode::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
#else
void LHWheelJointNode::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
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

bool LHWheelJointNode::lateLoading()
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
        
        b2WheelJointDef jointDef;
        
        jointDef.Initialize(bodyA, bodyB, posA, b2Vec2(-_axis.x,_axis.y));
        
        jointDef.enableMotor    = _enableMotor;
        jointDef.maxMotorTorque = _maxMotorTorque;
        jointDef.motorSpeed     = CC_DEGREES_TO_RADIANS(_motorSpeed*360.0);
        jointDef.frequencyHz    = _frequency;
        jointDef.dampingRatio   = _damping;
        
        jointDef.collideConnected = this->getCollideConnected();
        
        b2PrismaticJoint* joint = (b2PrismaticJoint*)world->CreateJoint(&jointDef);
        
        joint->SetUserData(this);
        this->setJoint(joint);
                
#else//chipmunk
        if(nodeA->getPhysicsBody() && nodeB->getPhysicsBody())
        {
            CCLOG("\n\nWARNING: Wheel joint is not supported when using Chipmunk physics engine.\n\n");
            
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
