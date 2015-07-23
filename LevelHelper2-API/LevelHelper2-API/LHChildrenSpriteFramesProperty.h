//
//  LHChildrenSpriteFramesProperty.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_CHILDREN_SPRITE_FRAMES_PROPERTY_H__
#define __LEVELHELPER_API_CHILDREN_SPRITE_FRAMES_PROPERTY_H__

#include "LHAnimationProperty.h"

class LHChildrenSpriteFramesProperty : public LHAnimationProperty
{
public:
    
    static LHChildrenSpriteFramesProperty* createWithDictionary(LHDictionary* dict, LHAnimation* anim);
    LHChildrenSpriteFramesProperty();
    virtual ~LHChildrenSpriteFramesProperty();
    bool initWithDictionary(LHDictionary* dict, LHAnimation* anim);
    
    LHAnimationProperty* newSubpropertyForNode(LHNodeAnimationProtocol* node);
    
    static  bool isLHChildrenSpriteFrameProperty(LHAnimationProperty* obj){return (0 != dynamic_cast<LHChildrenSpriteFramesProperty*>(obj));}

    
private:
    
};

#endif //__LEVELHELPER_API_CHILDREN_SPRITE_FRAMES_PROPERTY_H__
