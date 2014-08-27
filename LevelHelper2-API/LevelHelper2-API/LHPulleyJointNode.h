//
//  LHPulleyJointNode.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 27/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//
#ifndef __LEVELHELPER_API_PULLEY_JOINT_NODE_H__
#define __LEVELHELPER_API_PULLEY_JOINT_NODE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHJointsProtocol.h"
#include "LHConfig.h"

using namespace cocos2d;

/**
 LHPulleyJointNode class is used to load a LevelHelper pulley joint.
 
 WARNING - Pulley joint is not supported in Chipmunk.
 
 When using Box2d it uses a b2PulleyJoint.
 */

class LHDictionary;

class LHPulleyJointNode : public Node, public LHNodeProtocol, public LHJointsProtocol
{
public:
    
    static LHPulleyJointNode* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHPulleyJointNode();
    virtual ~LHPulleyJointNode();
        
    static  bool isLHPulleyJointNode(Node* obj){return (0 != dynamic_cast<LHPulleyJointNode*>(obj));}
    virtual bool isPulleyJointNode(){return true;}
    
    /**
     Returns the first ground anchor in scene coordinates.
     */
    Point getGroundAnchorA(){return _groundAnchorA;}
    
    /**
     Returns the first ground anchor in scene coordinates.
     */
    Point getGroundAnchorB(){return _groundAnchorB;}
    
#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif
    virtual bool lateLoading();
    
private:
    
    Point _groundAnchorA;
    Point _groundAnchorB;
    float   _ratio;
};

#endif //__LEVELHELPER_API_PULLEY_JOINT_NODE_H__
