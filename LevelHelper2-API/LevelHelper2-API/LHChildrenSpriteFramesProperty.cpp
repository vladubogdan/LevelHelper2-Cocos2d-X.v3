//
//  LHChildrenSpriteFramesProperty.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHChildrenSpriteFramesProperty.h"
#include "LHSpriteFrameProperty.h"


LHChildrenSpriteFramesProperty* LHChildrenSpriteFramesProperty::createWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    LHChildrenSpriteFramesProperty* aProp = new LHChildrenSpriteFramesProperty();
    
    if (aProp && aProp->initWithDictionary(dict, anim)){
        aProp->autorelease();
    }
    else{
        CC_SAFE_DELETE(aProp);
    }
    return aProp;
}
LHChildrenSpriteFramesProperty::LHChildrenSpriteFramesProperty(){

}
LHChildrenSpriteFramesProperty::~LHChildrenSpriteFramesProperty(){
    
}
bool LHChildrenSpriteFramesProperty::initWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    if(LHAnimationProperty::initWithDictionary(dict, anim))
    {
        //we may have something to init in the future
        return true;
    }
    return false;
}

LHAnimationProperty* LHChildrenSpriteFramesProperty::newSubpropertyForNode(LHNodeAnimationProtocol* node)
{
    LHSpriteFrameProperty* prop = LHSpriteFrameProperty::createWithDictionary(NULL, this->animation());
    if(prop){
        prop->setSubpropertyNode(node);
    }
    return prop;
}
