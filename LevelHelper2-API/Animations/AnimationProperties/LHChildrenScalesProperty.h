//
//  LHChildrenScalesProperty.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_CHILDREN_SCALES_PROPERTY_H__
#define __LEVELHELPER_API_CHILDREN_SCALES_PROPERTY_H__


#include "LHScaleProperty.h"

class LHChildrenScalesProperty : public LHScaleProperty
{
public:
    
    static LHChildrenScalesProperty* createWithDictionary(LHDictionary* dict, LHAnimation* anim);
    LHChildrenScalesProperty();
    virtual ~LHChildrenScalesProperty();
    bool initWithDictionary(LHDictionary* dict, LHAnimation* anim);
    
    LHAnimationProperty* newSubpropertyForNode(LHNodeAnimationProtocol* node);
    
    static  bool isLHChildrenScalesProperty(LHAnimationProperty* obj){return (0 != dynamic_cast<LHChildrenScalesProperty*>(obj));}
private:
};

#endif //__LEVELHELPER_API_CHILDREN_SCALES_PROPERTY_H__