//
//  LHOpacityProperty.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_ANIM_OPACITY_PROPERTY_H__
#define __LEVELHELPER_API_ANIM_OPACITY_PROPERTY_H__


#include "LHAnimationProperty.h"


class LHOpacityProperty : public LHAnimationProperty
{
public:
    
    static LHOpacityProperty* createWithDictionary(LHDictionary* dict, LHAnimation* anim);
    LHOpacityProperty();
    virtual ~LHOpacityProperty();
    bool initWithDictionary(LHDictionary* dict, LHAnimation* anim);
    
    static  bool isLHOpacityProperty(LHAnimationProperty* obj){return (0 != dynamic_cast<LHOpacityProperty*>(obj));}
private:
    
};

#endif //__LEVELHELPER_API_ANIM_OPACITY_PROPERTY_H__