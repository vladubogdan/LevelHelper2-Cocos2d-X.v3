//
//  LHRotationFrame.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHRotationFrame.h"
#include "LHDictionary.h"
#include "LHAnimationProperty.h"
//#include "LHAnimation.h"


LHRotationFrame* LHRotationFrame::createWithDictionary(LHDictionary* dict, LHAnimationProperty* prop)
{
    LHRotationFrame* frm = new LHRotationFrame();
    if (frm && frm->initWithDictionary(dict, prop)){
        frm->autorelease();
    }
    else{
        CC_SAFE_DELETE(frm);
    }
    return frm;
}

LHRotationFrame::LHRotationFrame(){
    
}
LHRotationFrame::~LHRotationFrame(){
    
}
bool LHRotationFrame::initWithDictionary(LHDictionary* dict, LHAnimationProperty* prop)
{
    if(LHFrame::initWithDictionary(dict, prop))
    {
        LHDictionary* rotInfo = dict->dictForKey("rotations");
        if(rotInfo)
        {
            __Array* allKeys = rotInfo->allKeys();
            if(allKeys)
            {
                for(int i = 0; i < allKeys->count();++i)
                {
                    std::string uuid = ((__String*)allKeys->getObjectAtIndex(i))->getCString();
                    float rot = rotInfo->floatForKey(uuid);
                    _rotations[uuid] = rot;
                }
            }
        }
        return true;
    }
    return false;
}

float LHRotationFrame::rotationForUUID(const std::string& uuid){
    return _rotations[uuid];
}
