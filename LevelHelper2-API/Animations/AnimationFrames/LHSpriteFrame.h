//
//  LHSpriteFrame.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_ANIM_SPRITEFRAME_FRAME_H__
#define __LEVELHELPER_API_ANIM_SPRITEFRAME_FRAME_H__

#include "LHFrame.h"

class LHSpriteFrame : public LHFrame
{
public:
    static LHSpriteFrame* createWithDictionary(LHDictionary* dict, LHAnimationProperty* prop);
    
    LHSpriteFrame();
    virtual ~LHSpriteFrame();
    virtual bool initWithDictionary(LHDictionary* dict, LHAnimationProperty* prop);
    
    std::string spriteFrameName(){return _spriteFrameName;}
    
private:
    std::string _spriteFrameName;
};

#endif //__LEVELHELPER_API_ANIM_SPRITEFRAME_FRAME_H__
