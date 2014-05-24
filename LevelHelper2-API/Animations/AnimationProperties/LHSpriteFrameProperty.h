//
//  LHSpriteFrameProperty.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_ANIM_SPRITE_FRAME_PROPERTY_H__
#define __LEVELHELPER_API_ANIM_SPRITE_FRAME_PROPERTY_H__


#include "LHAnimationProperty.h"

class LHSpriteFrameProperty : public LHAnimationProperty
{
public:
    
    static LHSpriteFrameProperty* createWithDictionary(LHDictionary* dict, LHAnimation* anim);
    LHSpriteFrameProperty();
    virtual ~LHSpriteFrameProperty();
    bool initWithDictionary(LHDictionary* dict, LHAnimation* anim);
    
    static  bool isLHSpriteFrameProperty(LHAnimationProperty* obj){return (0 != dynamic_cast<LHSpriteFrameProperty*>(obj));}
private:
    
};
#endif //__LEVELHELPER_API_ANIM_SPRITE_FRAME_PROPERTY_H__
