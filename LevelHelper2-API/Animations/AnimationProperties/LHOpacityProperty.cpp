//
//  LHOpacityProperty.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHOpacityProperty.h"
#include "LHOpacityFrame.h"

#include "LHDictionary.h"
#include "LHArray.h"

LHOpacityProperty* LHOpacityProperty::createWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    LHOpacityProperty* aProp = new LHOpacityProperty();
    
    if (aProp && aProp->initWithDictionary(dict, anim)){
        aProp->autorelease();
    }
    else{
        CC_SAFE_DELETE(aProp);
    }
    return aProp;
}
LHOpacityProperty::LHOpacityProperty(){
    
}
LHOpacityProperty::~LHOpacityProperty(){
    
}
bool LHOpacityProperty::initWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    if(dict == NULL)return true; //special case for children subproperties
    
    if(LHAnimationProperty::initWithDictionary(dict, anim))
    {
        LHArray* framesInfo = dict->arrayForKey("Frames");
        
        for(int i = 0; i < framesInfo->count(); ++i)
        {
            LHDictionary* frmInfo = framesInfo->dictAtIndex(i);
            
            LHOpacityFrame* frm = LHOpacityFrame::createWithDictionary(frmInfo, this);
            addKeyFrame(frm);
        }
        return true;
    }
    return false;
}
