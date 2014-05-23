//
//  LHSpriteFrame.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHSpriteFrame.h"
#include "LHDictionary.h"
#include "LHAnimationProperty.h"
//#include "LHAnimation.h"

LHSpriteFrame* LHSpriteFrame::createWithDictionary(LHDictionary* dict, LHAnimationProperty* prop)
{
    LHSpriteFrame* frm = new LHSpriteFrame();
    if (frm && frm->initWithDictionary(dict, prop)){
        frm->autorelease();
    }
    else{
        CC_SAFE_DELETE(frm);
    }
    return frm;
}

LHSpriteFrame::LHSpriteFrame(){
    
}
LHSpriteFrame::~LHSpriteFrame(){
    
}
bool LHSpriteFrame::initWithDictionary(LHDictionary* dict, LHAnimationProperty* prop)
{
    if(LHFrame::initWithDictionary(dict, prop))
    {
        _spriteFrameName = dict->stringForKey("spriteSheetName");
        return true;
    }
    return false;
}

