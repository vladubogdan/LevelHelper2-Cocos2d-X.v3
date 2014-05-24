//
//  LHScaleProperty.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_ANIM_SCALE_PROPERTY_H__
#define __LEVELHELPER_API_ANIM_SCALE_PROPERTY_H__


#include "LHAnimationProperty.h"

class LHScaleProperty : public LHAnimationProperty
{
public:
    
    static LHScaleProperty* createWithDictionary(LHDictionary* dict, LHAnimation* anim);
    LHScaleProperty();
    virtual ~LHScaleProperty();
    bool initWithDictionary(LHDictionary* dict, LHAnimation* anim);
    
    static  bool isLHScaleProperty(LHAnimationProperty* obj){return (0 != dynamic_cast<LHScaleProperty*>(obj));}
private:
    
};
#endif //__LEVELHELPER_API_ANIM_SCALE_PROPERTY_H__