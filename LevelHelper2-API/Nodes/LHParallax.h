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
    
    static LHParallax* parallaxWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHParallax();
    virtual ~LHParallax();

    bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    static  bool isLHParallax(Node* obj){return (0 != dynamic_cast<LHParallax*>(obj));}
    virtual bool isParallax(){return true;}
    
    //for some reason cocos2d-x people decided to make "visit()" method final - so we use this one instead
    virtual void visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated);
    
    
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
};

#endif //__LEVELHELPER_API_PARALLAX_H__
