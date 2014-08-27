//
//  LHGearJointNode.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 27/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//
#ifndef __LEVELHELPER_API_GEAR_JOINT_NODE_H__
#define __LEVELHELPER_API_GEAR_JOINT_NODE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHJointsProtocol.h"
#include "LHConfig.h"

using namespace cocos2d;

/**
 LHGearJointNode class is used to load a LevelHelper gear joint.
 
 WARNING - Weld joint is not supported in Chipmunk.
 
 When using Box2d it uses a b2GearJoint.
 */

class LHDictionary;

class LHGearJointNode : public Node, public LHNodeProtocol, public LHJointsProtocol
{
public:
    
    static LHGearJointNode* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    
    LHGearJointNode();
    virtual ~LHGearJointNode();
    
    
    static  bool isLHGearJointNode(Node* obj){return (0 != dynamic_cast<LHGearJointNode*>(obj));}
    virtual bool isGearJointNode(){return true;}
    
#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif
    virtual bool lateLoading();
    
private:
    
    void findConnectedJoints();
    
    std::string _jointAUUID;
    std::string _jointBUUID;
    
    Node* _jointA;
    Node* _jointB;
    
    float   _ratio;
};

#endif //__LEVELHELPER_API_GEAR_JOINT_NODE_H__
