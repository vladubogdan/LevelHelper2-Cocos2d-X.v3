//
//  LHChildrenRotationsProperty.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_CHILDREN_ROTATIONS_PROPERTY_H__
#define __LEVELHELPER_API_CHILDREN_ROTATIONS_PROPERTY_H__

#include "LHRotationProperty.h"

class LHChildrenRotationsProperty : public LHRotationProperty
{
public:
    
    static LHChildrenRotationsProperty* createWithDictionary(LHDictionary* dict, LHAnimation* anim);
    LHChildrenRotationsProperty();
    virtual ~LHChildrenRotationsProperty();
    bool initWithDictionary(LHDictionary* dict, LHAnimation* anim);
    
    virtual LHAnimationProperty* newSubpropertyForNode(LHNodeAnimationProtocol* node);
    
    static  bool isLHChildrenRotationsProperty(LHAnimationProperty* obj){return (0 != dynamic_cast<LHChildrenRotationsProperty*>(obj));}
    
private:
};

#endif //__LEVELHELPER_API_CHILDREN_ROTATIONS_PROPERTY_H__
