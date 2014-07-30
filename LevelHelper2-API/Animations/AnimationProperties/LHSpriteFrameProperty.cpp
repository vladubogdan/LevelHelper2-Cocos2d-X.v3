//
//  LHSpriteFrameProperty.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHSpriteFrameProperty.h"
#include "LHSpriteFrame.h"

#include "LHDictionary.h"
#include "LHArray.h"

LHSpriteFrameProperty* LHSpriteFrameProperty::createWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    LHSpriteFrameProperty* aProp = new LHSpriteFrameProperty();
    
    if (aProp && aProp->initWithDictionary(dict, anim)){
        aProp->autorelease();
    }
    else{
        CC_SAFE_DELETE(aProp);
    }
    return aProp;
}
LHSpriteFrameProperty::LHSpriteFrameProperty(){
    
}
LHSpriteFrameProperty::~LHSpriteFrameProperty(){
    
}
bool LHSpriteFrameProperty::initWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    if(dict == NULL)return true; //special case for children subproperties
    
    if(LHAnimationProperty::initWithDictionary(dict, anim))
    {
        LHArray* framesInfo = dict->arrayForKey("Frames");
        
        for(int i = 0; i < framesInfo->count(); ++i)
        {
            LHDictionary* frmInfo = framesInfo->dictAtIndex(i);
            
            LHSpriteFrame* frm = LHSpriteFrame::createWithDictionary(frmInfo, this);
            addKeyFrame(frm);
        }
        return true;
    }
    return false;
}
