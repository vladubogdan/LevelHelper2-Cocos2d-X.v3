//
//  LHNodeAnimationProtocol.h
//  LevelHelper2-Cocos2d-V3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_NODE_ANIMATION_PROTOCOL_H__
#define __LEVELHELPER_API_NODE_ANIMATION_PROTOCOL_H__

#include "cocos2d.h"
#include "LHUserPropertyProtocol.h"

class LHDictionary;
class LHArray;
class LHAnimation;
class LHSprite;

using namespace cocos2d;

/**
 LevelHelper 2 nodes that can be animated conform to this protocol.
 */

class LHNodeAnimationProtocol
{
public:
    
    LHNodeAnimationProtocol();
    virtual ~LHNodeAnimationProtocol();
    

    /**
     Set the active animation on a node.
     @param animation The animation that is about to get activated.
     */
    virtual void setActiveAnimation(LHAnimation* animation);

    
    
    virtual void createAnimationsFromDictionary(LHDictionary* dict);
    virtual void visitActiveAnimation();

private:
    __Array* _animations;
    LHAnimation* _activeAnimation;
};

#endif //__LEVELHELPER_API_NODE_ANIMATION_PROTOCOL_H__
