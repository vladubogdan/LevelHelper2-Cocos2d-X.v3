//
//  LHChildrenScalesProperty.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHChildrenScalesProperty.h"
#include "LHScaleFrame.h"
#include "LHScaleProperty.h"

LHChildrenScalesProperty* LHChildrenScalesProperty::createWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    LHChildrenScalesProperty* aProp = new LHChildrenScalesProperty();
    
    if (aProp && aProp->initWithDictionary(dict, anim)){
        aProp->autorelease();
    }
    else{
        CC_SAFE_DELETE(aProp);
    }
    return aProp;
}
LHChildrenScalesProperty::LHChildrenScalesProperty(){
    
}
LHChildrenScalesProperty::~LHChildrenScalesProperty(){
    
}
bool LHChildrenScalesProperty::initWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    if(LHScaleProperty::initWithDictionary(dict, anim))
    {
        //we may have something to init in the future
        return true;
    }
    return false;
}

LHAnimationProperty* LHChildrenScalesProperty::newSubpropertyForNode(LHNodeAnimationProtocol* node)
{
    LHScaleProperty* prop = LHScaleProperty::createWithDictionary(NULL, this->animation());
    if(prop){
        prop->setSubpropertyNode(node);
    }
    return prop;
}
