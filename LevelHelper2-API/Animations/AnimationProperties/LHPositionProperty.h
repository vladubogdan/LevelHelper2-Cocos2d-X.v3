//
//  LHPositionProperty.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//
#ifndef __LEVELHELPER_API_ANIM_POSITION_PROPERTY_H__
#define __LEVELHELPER_API_ANIM_POSITION_PROPERTY_H__


#include "LHAnimationProperty.h"

class LHPositionProperty : public LHAnimationProperty
{
public:
    
    static LHPositionProperty* createWithDictionary(LHDictionary* dict, LHAnimation* anim);
    LHPositionProperty();
    virtual ~LHPositionProperty();
    bool initWithDictionary(LHDictionary* dict, LHAnimation* anim);

    static  bool isLHPositionProperty(LHAnimationProperty* obj){return (0 != dynamic_cast<LHPositionProperty*>(obj));}

private:
    
    
};

#endif //__LEVELHELPER_API_ANIM_POSITION_PROPERTY_H__