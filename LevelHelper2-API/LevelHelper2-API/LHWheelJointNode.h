//
//  LHWheelJointNode.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 27/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//
#ifndef __LEVELHELPER_API_WHEEL_JOINT_NODE_H__
#define __LEVELHELPER_API_WHEEL_JOINT_NODE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHJointsProtocol.h"
#include "LHConfig.h"

using namespace cocos2d;

/**
 LHWheelJointNode class is used to load a LevelHelper wheel joint.
 
 WARNING - Prismatic joint is not supported in Chipmunk.
 
 When using Box2d it uses a b2WheelJoint.
 */

class LHDictionary;
class LHScene;

class LHWheelJointNode : public Node, public LHNodeProtocol, public LHJointsProtocol
{
public:
    
    static LHWheelJointNode* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHWheelJointNode();
    virtual ~LHWheelJointNode();
        
    static  bool isLHWheelJointNode(Node* obj){return (0 != dynamic_cast<LHWheelJointNode*>(obj));}
    virtual bool isWheelJointNode(){return true;}
    
    /**
     Returns whether or not the motor is enabled on the joint.
     */
    bool getEnableMotor(){return _enableMotor;}
    
    /**
     Returns the maximum motor torque.
     */
    float getMaxMotorTorque(){return _maxMotorTorque;}
    
    /**
     Returns the motor speed in degrees.
     */
    float getMotorSpeed(){return _motorSpeed;}
    
    /**
     Returns the frequency used on the joint.
     */
    float getFrequency(){return _frequency;}
    
    /**
     Returns the damping ratio used on the joint.
     */
    float getDampingRatio(){return _damping;}
    
    /**
     Returns the axis on which this joint is moving.
     */
    Point getAxis(){return _axis;}
    
    
#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif
    
    virtual bool lateLoading();
    
private:
    
    bool _enableMotor;
    
    float _maxMotorTorque;
    float _motorSpeed;
    
    float _frequency;
    float _damping;
    
    Point _axis;
};

#endif //__LEVELHELPER_API_WHEEL_JOINT_NODE_H__
