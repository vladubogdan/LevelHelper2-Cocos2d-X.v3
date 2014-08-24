//
//  LHCameraActivateProperty.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_ANIM_CAMERA_PROPERTY_H__
#define __LEVELHELPER_API_ANIM_CAMERA_PROPERTY_H__

#include "LHAnimationProperty.h"

class LHCameraActivateProperty : public LHAnimationProperty
{
public:
    
    static LHCameraActivateProperty* createWithDictionary(LHDictionary* dict, LHAnimation* anim);
    LHCameraActivateProperty();
    virtual ~LHCameraActivateProperty();
    bool initWithDictionary(LHDictionary* dict, LHAnimation* anim);
    
private:
    
};

#endif //__LEVELHELPER_API_ANIM_CAMERA_PROPERTY_H__
