//
//  LHWeldJointNode.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 27/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//
#ifndef __LEVELHELPER_API_WELD_JOINT_NODE_H__
#define __LEVELHELPER_API_WELD_JOINT_NODE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHJointsProtocol.h"
#include "LHConfig.h"

using namespace cocos2d;

/**
 LHWeldJointNode class is used to load a LevelHelper weld joint.
 
 WARNING - Weld joint is not supported in Chipmunk.
 
 When using Box2d it uses a b2WeldJoint.
 */

class LHDictionary;

class LHWeldJointNode : public Node, public LHNodeProtocol, public LHJointsProtocol
{
public:
    
    static LHWeldJointNode* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHWeldJointNode();
    virtual ~LHWeldJointNode();
    
    
    static  bool isLHWeldJointNode(Node* obj){return (0 != dynamic_cast<LHWeldJointNode*>(obj));}
    virtual bool isWeldJointNode(){return true;}
    
    /**
     Returns the frequency used by this joint.
     */
    float getFrequency(){return _frequency;}
    
    /**
     Returns the damping ratio used by this joint.
     */
    float getDampingRatio(){return _damping;}
    

#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif
    
    virtual bool lateLoading();
    
private:
    
    float _frequency;
    float _damping;
    
};

#endif //__LEVELHELPER_API_WELD_JOINT_NODE_H__
