//
//  LHScaleFrame.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHScaleFrame.h"
#include "LHDictionary.h"
#include "LHAnimationProperty.h"
//#include "LHAnimation.h"


LHScaleFrame* LHScaleFrame::createWithDictionary(LHDictionary* dict, LHAnimationProperty* prop)
{
    LHScaleFrame* frm = new LHScaleFrame();
    if (frm && frm->initWithDictionary(dict, prop)){
        frm->autorelease();
    }
    else{
        CC_SAFE_DELETE(frm);
    }
    return frm;
}

LHScaleFrame::LHScaleFrame(){
    
}
LHScaleFrame::~LHScaleFrame(){
    
}
bool LHScaleFrame::initWithDictionary(LHDictionary* dict, LHAnimationProperty* prop)
{
    if(LHFrame::initWithDictionary(dict, prop))
    {
        LHDictionary* scalesInfo = dict->dictForKey("scales");
        __Array* allKeys = scalesInfo->allKeys();
        for(int i = 0; i < allKeys->count();++i)
        {
            std::string uuid = ((__String*)allKeys->getObjectAtIndex(i))->getCString();
            Size scl = scalesInfo->sizeForKey(uuid);
            _scales[uuid] = scl;
        }
        
        return true;
    }
    return false;
}

Size LHScaleFrame::scaleForUUID(const std::string& uuid){
    return _scales[uuid];
}
