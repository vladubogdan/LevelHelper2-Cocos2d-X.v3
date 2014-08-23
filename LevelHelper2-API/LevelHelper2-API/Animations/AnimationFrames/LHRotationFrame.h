//
//  LHRotationFrame.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_ANIM_ROTATION_FRAME_H__
#define __LEVELHELPER_API_ANIM_ROTATION_FRAME_H__

#include "LHFrame.h"

class LHRotationFrame : public LHFrame
{
public:
    
    static LHRotationFrame* createWithDictionary(LHDictionary* dict, LHAnimationProperty* prop);
    
    LHRotationFrame();
    virtual ~LHRotationFrame();
    virtual bool initWithDictionary(LHDictionary* dict, LHAnimationProperty* prop);
    
    
    float rotationForUUID(const std::string& uuid);
    
private:
    std::map<std::string, float>_rotations;
};
#endif //__LEVELHELPER_API_ANIM_ROTATION_FRAME_H__
