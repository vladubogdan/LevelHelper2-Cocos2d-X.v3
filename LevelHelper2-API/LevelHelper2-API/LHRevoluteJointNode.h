//
//  LHRevoluteJointNode.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 27/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//
#ifndef __LEVELHELPER_API_REVOLUTE_JOINT_NODE_H__
#define __LEVELHELPER_API_REVOLUTE_JOINT_NODE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHJointsProtocol.h"
#include "LHConfig.h"

using namespace cocos2d;

/**
 LHRevoluteJointNode class is used to load a LevelHelper revolute joint.
 
 WARNING - Revolute joint is not supported in Chipmunk.
 
 When using Box2d it uses a b2RevoluteJoint.
 */

class LHDictionary;

class LHRevoluteJointNode : public Node, public LHNodeProtocol, public LHJointsProtocol
{
public:
    
    static LHRevoluteJointNode* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    
    LHRevoluteJointNode();
    virtual ~LHRevoluteJointNode();
    
    
    static  bool isLHRevoluteJointNode(Node* obj){return (0 != dynamic_cast<LHRevoluteJointNode*>(obj));}
    virtual bool isRevoluteJointNode(){return true;}
    
    /**
     Returns whether or not the limit is enabled on the joint.
     */
    bool getEnableLimit(){return _enableLimit;}
    
    /**
     Returns whether or not the motor is enabled on the joint.
     */
    bool getEnableMotor(){return _enableMotor;}
    
    /**
     Returns the lower angle limit
     */
    float getLowerAngle(){return _lowerAngle;}
    
    /**
     Returns the upper angle limit
     */
    float getUpperAngle(){return _upperAngle;}
    
    /**
     Returns the maximum motor torque
     */
    float getMaxMotorTorque(){return _maxMotorTorque;}
    
    /**
     Returns the motor speed.
     */
    float getMotorSpeed(){return _motorSpeed;}
    
    virtual bool lateLoading();

#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif

private:
    
    bool _enableLimit;
    bool _enableMotor;
    
    float _lowerAngle;
    float _upperAngle;
    
    float _maxMotorTorque;
    float _motorSpeed;
    
};

#endif //__LEVELHELPER_API_REVOLUTE_JOINT_NODE_H__
