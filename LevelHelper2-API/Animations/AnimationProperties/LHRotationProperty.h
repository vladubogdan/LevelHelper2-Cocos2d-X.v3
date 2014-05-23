//
//  LHRotationProperty.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_ANIM_ROTATION_PROPERTY_H__
#define __LEVELHELPER_API_ANIM_ROTATION_PROPERTY_H__


#include "LHAnimationProperty.h"

class LHRotationProperty : public LHAnimationProperty
{
public:
    
    static LHRotationProperty* createWithDictionary(LHDictionary* dict, LHAnimation* anim);
    LHRotationProperty();
    virtual ~LHRotationProperty();
    bool initWithDictionary(LHDictionary* dict, LHAnimation* anim);
    
    static  bool isLHRotationProperty(LHAnimationProperty* obj){return (0 != dynamic_cast<LHRotationProperty*>(obj));}
    
private:
    
};

#endif //__LEVELHELPER_API_ANIM_ROTATION_PROPERTY_H__