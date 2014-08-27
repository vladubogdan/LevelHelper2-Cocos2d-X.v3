//
//  LHPrismaticJointNode.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 27/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//
#ifndef __LEVELHELPER_API_PRISMATIC_JOINT_NODE_H__
#define __LEVELHELPER_API_PRISMATIC_JOINT_NODE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHJointsProtocol.h"
#include "LHConfig.h"

using namespace cocos2d;

/**
 LHPrismaticJointNode class is used to load a LevelHelper prismatic joint.
 
 WARNING - Prismatic joint is not supported in Chipmunk.
 
 When using Box2d it uses a b2PrismaticJoint.
 */

class LHDictionary;

class LHPrismaticJointNode : public Node, public LHNodeProtocol, public LHJointsProtocol
{
public:
    
    static LHPrismaticJointNode* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    
    LHPrismaticJointNode();
    virtual ~LHPrismaticJointNode();
    
    
    static  bool isLHPrismaticJointNode(Node* obj){return (0 != dynamic_cast<LHPrismaticJointNode*>(obj));}
    virtual bool isPrismaticJointNode(){return true;}
    
    /**
     Returns whether or not the limit is enabled on the joint.
     */
    bool getEnableLimit(){return _enableLimit;}
    
    /**
     Returns whether or not the motor is enabled on the joint.
     */
    bool getEnableMotor(){return _enableMotor;}
    
    /**
     Returns the lower translation limit.
     */
    float getLowerTranslation(){return _lowerTranslation;}
    
    /**
     Returns the upper translation limit.
     */
    float getUpperTranslation(){return _upperTranslation;}
    
    /**
     Returns the maximum motor force.
     */
    float getMaxMotorForce(){return _maxMotorForce;}
    
    /**
     Returns the motor speed in degrees.
     */
    float getMotorSpeed(){return _motorSpeed;}
    
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
    
    bool _enableLimit;
    bool _enableMotor;
    
    float _lowerTranslation;
    float _upperTranslation;
    
    float _maxMotorForce;
    float _motorSpeed;
    
    Point _axis;
};

#endif //__LEVELHELPER_API_PRISMATIC_JOINT_NODE_H__
