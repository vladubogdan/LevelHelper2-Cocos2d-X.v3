//
//  LHOpacityFrame.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHOpacityFrame.h"
#include "LHDictionary.h"
#include "LHAnimationProperty.h"
//#include "LHAnimation.h"



LHOpacityFrame* LHOpacityFrame::createWithDictionary(LHDictionary* dict,
                                                     LHAnimationProperty* prop)
{
    LHOpacityFrame* frm = new LHOpacityFrame();
    if (frm && frm->initWithDictionary(dict, prop)){
        frm->autorelease();
    }
    else{
        CC_SAFE_DELETE(frm);
    }
    return frm;
}

LHOpacityFrame::LHOpacityFrame()
{
    
}
LHOpacityFrame::~LHOpacityFrame()
{
    
}
bool LHOpacityFrame::initWithDictionary(LHDictionary* dict, LHAnimationProperty* prop)
{
    if(LHFrame::initWithDictionary(dict, prop))
    {
        LHDictionary* opaInfo = dict->dictForKey("opacities");
        if(opaInfo)
        {
            __Array* allKeys = opaInfo->allKeys();
            if(allKeys)
            {
                for(int i = 0; i < allKeys->count();++i)
                {
                    std::string uuid = ((__String*)allKeys->getObjectAtIndex(i))->getCString();
                    float op = opaInfo->floatForKey(uuid);
                    opacities[uuid] = op;
                }
            }
        }
        
        return true;
    }
    return false;
}

float LHOpacityFrame::opacityForUUID(const std::string& uuid){
    return opacities[uuid];
}
