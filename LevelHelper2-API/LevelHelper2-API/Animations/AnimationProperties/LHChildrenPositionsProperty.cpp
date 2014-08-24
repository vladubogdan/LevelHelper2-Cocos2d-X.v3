//
//  LHChildrenPositionsProperty.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHChildrenPositionsProperty.h"
#include "LHPositionFrame.h"
#include "LHPositionProperty.h"


LHChildrenPositionsProperty* LHChildrenPositionsProperty::createWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    LHChildrenPositionsProperty* aProp = new LHChildrenPositionsProperty();
    
    if (aProp && aProp->initWithDictionary(dict, anim)){
        aProp->autorelease();
    }
    else{
        CC_SAFE_DELETE(aProp);
    }
    return aProp;
}
LHChildrenPositionsProperty::LHChildrenPositionsProperty(){

}
LHChildrenPositionsProperty::~LHChildrenPositionsProperty(){
    
}
bool LHChildrenPositionsProperty::initWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    if(LHPositionProperty::initWithDictionary(dict, anim))
    {
        //we may have something to init in the future
        return true;
    }
    return false;
}

LHAnimationProperty* LHChildrenPositionsProperty::newSubpropertyForNode(LHNodeAnimationProtocol* node)
{
    LHPositionProperty* prop = LHPositionProperty::createWithDictionary(NULL, this->animation());
    if(prop){
        prop->setSubpropertyNode(node);
    }
    return prop;
}
