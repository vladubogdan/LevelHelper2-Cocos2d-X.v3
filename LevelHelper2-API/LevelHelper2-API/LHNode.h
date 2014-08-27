//
//  LHNode.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_NODE_H__
#define __LEVELHELPER_API_NODE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHPhysicsProtocol.h"
#include "LHNodeAnimationProtocol.h"
#include "LHConfig.h"

using namespace cocos2d;

/**
 LHNode class is used to load a node object from a level file.
 Users can retrieve node objects by calling the scene (LHScene) getChildNodeWithName("name") method.
 */

class LHDictionary;
class LHScene;

class LHNode : public Node, public LHNodeProtocol, public LHNodeAnimationProtocol, public LHPhysicsProtocol
{
public:
 
    static LHNode* createWithName(const std::string& nm);
    virtual bool initWithName(const std::string& nm);
        
    static LHNode* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHNode();
    virtual ~LHNode();
    
    static  bool isLHNode(Node* obj){return (0 != dynamic_cast<LHNode*>(obj));}
    virtual bool isNode(){return true;}
    

#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif

    
#if LH_USE_BOX2D
    virtual void setPosition(const cocos2d::Vec2 &pos);
    virtual void setRotation(float rotation);
    virtual void setScaleX(float scaleX);
    virtual void setScaleY(float scaleY);
    virtual void setScale(float scaleX, float scaleY);
    virtual void updatePosition(const cocos2d::Vec2 &pos);
    virtual void updateRotation(float rotation);
#endif
    
};

#endif //__LEVELHELPER_API_NODE_H__
