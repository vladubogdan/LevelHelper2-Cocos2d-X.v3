//
//  LHAnimationProperty.cpp
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHAnimationProperty.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHAnimation.h"
#include "LHFrame.h"
#include "LHNodeAnimationProtocol.h"
#include "LHNodeProtocol.h"

#include "LHPositionProperty.h"
#include "LHChildrenPositionsProperty.h"

#include "LHRotationProperty.h"
#include "LHChildrenRotationsProperty.h"

#include "LHScaleProperty.h"
#include "LHChildrenScalesProperty.h"

#include "LHOpacityProperty.h"
#include "LHChildrenOpacitiesProperty.h"

#include "LHSpriteFrameProperty.h"
#include "LHChildrenSpriteFramesProperty.h"

#include "LHCameraActivateProperty.h"

#include "LHRootBoneProperty.h"

LHAnimationProperty::~LHAnimationProperty()
{
    _parentProperty  = NULL;
    _subpropertyNode = NULL;
    _animation      = NULL;

    CC_SAFE_DELETE(_frames);
    CC_SAFE_DELETE(subproperties);
}

LHAnimationProperty* LHAnimationProperty::createWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    
    std::string type = dict->stringForKey("type");
    
//    printf("ANIMTION PROP TYPE %s\n", type.c_str());
    
    LHAnimationProperty* aProp = NULL;
    if(type == "LHPositionProperty"){
        aProp = new LHPositionProperty();
    }
    else if(type == "LHChildrenPositionsProperty"){
        aProp = new LHChildrenPositionsProperty();
    }
    else if(type == "LHRotationProperty"){
        aProp = new LHRotationProperty();
    }
    else if(type == "LHChildrenRotationsProperty"){
        aProp = new LHChildrenRotationsProperty();
    }
    else if(type == "LHScaleProperty"){
        aProp = new LHScaleProperty();
    }
    else if(type == "LHChildrenScalesProperty"){
        aProp = new LHChildrenScalesProperty();
    }
    else if(type == "LHOpacityProperty"){
        aProp = new LHOpacityProperty();
    }
    else if(type == "LHChildrenOpacitiesProperty"){
        aProp = new LHChildrenOpacitiesProperty();
    }
    else if(type == "LHSpriteFrameProperty"){
        aProp = new LHSpriteFrameProperty();
    }
    else if(type == "LHChildrenSpriteFramesProperty"){
        aProp = new LHChildrenSpriteFramesProperty();
    }
    else if(type == "LHRootBoneProperty"){
        aProp = new LHRootBoneProperty();
    }
    else if(type == "LHCameraActivateProperty"){
        aProp = new LHCameraActivateProperty();
    }
    else{
        CCLOG("COULD NOT FIND ANIMATION PROPETY %s", type.c_str());
        
        aProp = new LHAnimationProperty();
    }
    
    if (aProp && aProp->initWithDictionary(dict, anim)){
        aProp->autorelease();
    }
    else{
        CC_SAFE_DELETE(aProp);
    }
    return aProp;
}

LHAnimationProperty::LHAnimationProperty(){

    _frames = new __Array();
    _frames->init();
    
    _parentProperty = NULL;
    _subpropertyNode = NULL;
    subproperties = NULL;
    _animation = NULL;
}

bool LHAnimationProperty::initWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    if(!dict)return false;
    
    _animation = anim;
    
    LHDictionary* subsInfo = dict->dictForKey("subproperties");
    if(subsInfo)
    {
        __Array* allKeys = subsInfo->allKeys();
        
        LHNodeAnimationProtocol* animNode = _animation->node();
        
        LHNodeProtocol* parentNode = dynamic_cast<LHNodeProtocol*>(animNode);
        
        if(!parentNode)
            return false;
        
        for(int i = 0; i < allKeys->count(); ++i)
        {
            std::string subUUID = ((__String*)allKeys->getObjectAtIndex(i))->getCString();
            
            LHDictionary* subInfo = subsInfo->dictForKey(subUUID);
            if(subInfo)
            {
                Node* returnedNode = parentNode->getChildNodeWithUUID(subUUID);
                
                LHNodeAnimationProtocol* child = dynamic_cast<LHNodeAnimationProtocol*>(returnedNode);
                
                if(child && subInfo){
                    
                    if(!subproperties){
                        subproperties = new __Dictionary();
                        subproperties->init();
                    }
                    
                    LHAnimationProperty* subProp = newSubpropertyForNode(child);
                    if(subProp){
                
                        subProp->setParentProperty(this);
                        subProp->setSubpropertyNode(child);
                        subProp->initWithDictionary(subInfo, anim);
                        
                        LHNodeProtocol* childNode = dynamic_cast<LHNodeProtocol*>(child);
                        
                        if(childNode)
                        {
                            subproperties->setObject(subProp, childNode->getUuid());
                        }
                    }
                }
            }
        }
    }
    
    return true;
}

void LHAnimationProperty::addKeyFrame(LHFrame* frm)
{
    if(!frm)return;

    if(!_frames->containsObject(frm)){
        _frames->addObject(frm);
    }
}

__Array* LHAnimationProperty::keyFrames(){
    return _frames;
}

LHAnimation* LHAnimationProperty::animation(){
    return _animation;
}

#pragma mark - SUBPROPERTIES SUPPORT
bool LHAnimationProperty::isSubproperty(){
    return _parentProperty != NULL;
}
LHNodeAnimationProtocol* LHAnimationProperty::subpropertyNode(){
    return _subpropertyNode;
}
void LHAnimationProperty::setSubpropertyNode(LHNodeAnimationProtocol* val){
    _subpropertyNode = val;
}

LHAnimationProperty* LHAnimationProperty::parentProperty(){
    return _parentProperty;
}
void LHAnimationProperty::setParentProperty(LHAnimationProperty* val){
    _parentProperty = val;
}

bool LHAnimationProperty::canHaveSubproperties(){
    return false;
}
void LHAnimationProperty::addSubproperty(LHAnimationProperty* prop){
    if(!prop)return;
    
    std::string subUuid = ((LHNodeProtocol*)prop->subpropertyNode())->getUuid();
    if(subUuid.length()>0){
        if(!subproperties){
            subproperties = new __Dictionary();;
        }
        
        prop->setParentProperty(this);
        subproperties->setObject(prop, subUuid);
    }
}
void LHAnimationProperty::removeSubproperty(LHAnimationProperty* prop){
    if(!prop)return;
    
    __Array* keys = subproperties->allKeys();
    
    for(int i = 0; i < keys->count(); ++i)
    {
        std::string key = ((__String*)keys->getObjectAtIndex(i))->getCString();
    
        if(subproperties->objectForKey(key) == prop){
            subproperties->removeObjectForKey(key);
            return;
        }
    }
}

LHAnimationProperty* LHAnimationProperty::newSubpropertyForNode(LHNodeAnimationProtocol* node)
{
    printf("SUBCLASSERS NEED TO IMPLEMENT newSubpropertyForNode\n");
    return NULL;
}

__Array* LHAnimationProperty::allSubproperties(){
    
    __Array* newArray = __Array::create();
    
    if(subproperties)
    {
        __Array* keys = subproperties->allKeys();
        if(keys)
        {
            for(int i = 0; i < keys->count(); ++i)
            {
                std::string key = ((__String*)keys->getObjectAtIndex(i))->getCString();
                
                Ref* obj = subproperties->objectForKey(key);
                if(obj){
                    newArray->addObject(obj);
                }
            }
        }
    }
    return newArray;
}

LHAnimationProperty* LHAnimationProperty::subpropertyForUUID(const std::string& nodeUuid){
    if(subproperties){
        return (LHAnimationProperty*)subproperties->objectForKey(nodeUuid);
    }
    return NULL;
}
