//
//  LHRootBoneProperty.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//
#ifndef __LEVELHELPER_API_ANIM_ROOT_BONE_PROPERTY_H__
#define __LEVELHELPER_API_ANIM_ROOT_BONE_PROPERTY_H__


#include "LHAnimationProperty.h"

class LHRootBoneProperty : public LHAnimationProperty
{
public:
    
    static LHRootBoneProperty* createWithDictionary(LHDictionary* dict, LHAnimation* anim);
    LHRootBoneProperty();
    virtual ~LHRootBoneProperty();
    bool initWithDictionary(LHDictionary* dict, LHAnimation* anim);

    static bool isLHRootBoneProperty(LHAnimationProperty* obj){return (0 != dynamic_cast<LHRootBoneProperty*>(obj));}

private:
    
    
};

#endif //__LEVELHELPER_API_ANIM_ROOT_BONE_PROPERTY_H__