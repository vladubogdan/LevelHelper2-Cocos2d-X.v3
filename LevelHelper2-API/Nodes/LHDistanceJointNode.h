//
//  LHDistanceJointNode.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 27/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//
#ifndef __LEVELHELPER_API_DISTANCE_JOINT_NODE_H__
#define __LEVELHELPER_API_DISTANCE_JOINT_NODE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHJointsProtocol.h"
#include "LHConfig.h"

using namespace cocos2d;

/**
 LHDistanceJointNode class is used to load a LevelHelper distance joint.
 
 When using Cocos2d/Chipmunk it uses a CCPhysicsSlideJoint joint.
 
 When using Box2d it uses a b2DistanceJoint object.
 */

class LHDictionary;

class LHDistanceJointNode : public Node, public LHNodeProtocol, public LHJointsProtocol
{
public:
    
    static LHDistanceJointNode* distanceJointNodeWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHDistanceJointNode();
    virtual ~LHDistanceJointNode();
    bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    static  bool isLHDistanceJointNode(Node* obj){return (0 != dynamic_cast<LHDistanceJointNode*>(obj));}
    virtual bool isDistanceJointNode(){return true;}
    
    virtual void removeFromParent();
    
    /**
     Returns the damping ratio of the joint.
     */
    float getDamping(){return _dampingRatio;}
    
    /**
     Returns the frequency of the joint.
     */
    float getFrequency(){return _frequency;}
    
    
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
    virtual bool lateLoading();
    
private:
    
    float _dampingRatio;
    float _frequency;    
};

#endif //__LEVELHELPER_API_DISTANCE_JOINT_NODE_H__
