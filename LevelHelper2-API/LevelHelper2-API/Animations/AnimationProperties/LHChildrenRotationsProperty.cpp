//
//  LHChildrenRotationsProperty.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHChildrenRotationsProperty.h"
#include "LHRotationFrame.h"
#include "LHRotationProperty.h"


LHChildrenRotationsProperty* LHChildrenRotationsProperty::createWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    LHChildrenRotationsProperty* aProp = new LHChildrenRotationsProperty();
    
    if (aProp && aProp->initWithDictionary(dict, anim)){
        aProp->autorelease();
    }
    else{
        CC_SAFE_DELETE(aProp);
    }
    return aProp;
}
LHChildrenRotationsProperty::LHChildrenRotationsProperty(){
    
}
LHChildrenRotationsProperty::~LHChildrenRotationsProperty(){
    
}
bool LHChildrenRotationsProperty::initWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    if(LHRotationProperty::initWithDictionary(dict, anim))
    {
        //we may have something to init in the future
        return true;
    }
    return false;
}

LHAnimationProperty* LHChildrenRotationsProperty::newSubpropertyForNode(LHNodeAnimationProtocol* node)
{
    LHRotationProperty* prop = LHRotationProperty::createWithDictionary(NULL, this->animation());
    if(prop){
        prop->setSubpropertyNode(node);
    }
    return prop;
}

