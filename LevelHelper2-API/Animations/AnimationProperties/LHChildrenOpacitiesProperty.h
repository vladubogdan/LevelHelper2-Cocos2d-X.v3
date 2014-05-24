//
//  LHChildrenOpacitiesProperty.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_CHILDREN_OPACITIES_PROPERTY_H__
#define __LEVELHELPER_API_CHILDREN_OPACITIES_PROPERTY_H__

#include "LHOpacityProperty.h"

class LHChildrenOpacitiesProperty : public LHOpacityProperty
{
public:
    
    static LHChildrenOpacitiesProperty* createWithDictionary(LHDictionary* dict, LHAnimation* anim);
    LHChildrenOpacitiesProperty();
    virtual ~LHChildrenOpacitiesProperty();
    bool initWithDictionary(LHDictionary* dict, LHAnimation* anim);
    
    virtual LHAnimationProperty* newSubpropertyForNode(LHNodeAnimationProtocol* node);
    
    static  bool isLHChildrenOpacitiesProperty(LHAnimationProperty* obj){return (0 != dynamic_cast<LHChildrenOpacitiesProperty*>(obj));}
private:
    
};

#endif //__LEVELHELPER_API_CHILDREN_OPACITIES_PROPERTY_H__
