//
//  LHParallax.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_PARALLAX_H__
#define __LEVELHELPER_API_PARALLAX_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHNodeAnimationProtocol.h"

using namespace cocos2d;

/**
 LHParallax class is used to load a parallax object from a level file.
 Users can retrieve node objects by calling the scene (LHScene) childNodeWithName: method.
 */

class LHDictionary;
class LHScene;

class LHParallax : public Node, public LHNodeProtocol, public LHNodeAnimationProtocol
{
public:
    
    static LHParallax* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHParallax();
    virtual ~LHParallax();

    static  bool isLHParallax(Node* obj){return (0 != dynamic_cast<LHParallax*>(obj));}
    virtual bool isParallax(){return true;}
    
#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif

    
    /**
     Returns the followed node or nullptr if no node is being fallowed;
     */
    Node* followedNode();
    
    /**
     Set a node that should be followed by this parallax.
     @param node The node that should be followed by the parallax. Usually a camera node.
     */
    void followNode(Node* node);

private:
    
    void transformLayerPositions();
    
    std::string _followedNodeUUID;
    Node* _followedNode;
    Point lastPosition;
    Point initialPosition;
};

#endif //__LEVELHELPER_API_PARALLAX_H__
