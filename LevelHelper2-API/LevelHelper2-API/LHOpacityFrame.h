//
//  LHOpacityFrame.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_ANIM_OPACITY_FRAME_H__
#define __LEVELHELPER_API_ANIM_OPACITY_FRAME_H__

#include "LHFrame.h"

class LHOpacityFrame : public LHFrame
{
public:

    static LHOpacityFrame* createWithDictionary(LHDictionary* dict, LHAnimationProperty* prop);
    
    LHOpacityFrame();
    virtual ~LHOpacityFrame();
    virtual bool initWithDictionary(LHDictionary* dict, LHAnimationProperty* prop);
    

    float opacityForUUID(const std::string& uuid);
    
private:
    std::map<std::string, float> opacities;
};

#endif //__LEVELHELPER_API_ANIM_OPACITY_FRAME_H__