//
//  LHChildrenOpacitiesProperty.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHChildrenOpacitiesProperty.h"
#include "LHOpacityFrame.h"
#include "LHOpacityProperty.h"

LHChildrenOpacitiesProperty* LHChildrenOpacitiesProperty::createWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    LHChildrenOpacitiesProperty* aProp = new LHChildrenOpacitiesProperty();
    
    if (aProp && aProp->initWithDictionary(dict, anim)){
        aProp->autorelease();
    }
    else{
        CC_SAFE_DELETE(aProp);
    }
    return aProp;
}
LHChildrenOpacitiesProperty::LHChildrenOpacitiesProperty(){
    
}
LHChildrenOpacitiesProperty::~LHChildrenOpacitiesProperty(){
    
}
bool LHChildrenOpacitiesProperty::initWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    if(LHOpacityProperty::initWithDictionary(dict, anim))
    {
        //we may have something to init in the future
        return true;
    }
    return false;
}

LHAnimationProperty* LHChildrenOpacitiesProperty::newSubpropertyForNode(LHNodeAnimationProtocol* node)
{
    LHOpacityProperty* prop = LHOpacityProperty::createWithDictionary(NULL, this->animation());
    prop->setSubpropertyNode(node);
    return prop;
}
