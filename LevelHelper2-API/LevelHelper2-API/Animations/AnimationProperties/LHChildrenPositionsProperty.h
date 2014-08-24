//
//  LHChildrenPositionsProperty.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_CHILDREN_POSITIONS_PROPERTY_H__
#define __LEVELHELPER_API_CHILDREN_POSITIONS_PROPERTY_H__

#include "LHPositionProperty.h"

class LHChildrenPositionsProperty : public LHPositionProperty
{
public:
    
    static LHChildrenPositionsProperty* createWithDictionary(LHDictionary* dict, LHAnimation* anim);
    LHChildrenPositionsProperty();
    virtual ~LHChildrenPositionsProperty();
    bool initWithDictionary(LHDictionary* dict, LHAnimation* anim);
    
    LHAnimationProperty* newSubpropertyForNode(LHNodeAnimationProtocol* node);
    
    static  bool isLHChildrenPositionsProperty(LHAnimationProperty* obj){return (0 != dynamic_cast<LHChildrenPositionsProperty*>(obj));}

    
private:
    
};

#endif //__LEVELHELPER_API_CHILDREN_POSITIONS_PROPERTY_H__
