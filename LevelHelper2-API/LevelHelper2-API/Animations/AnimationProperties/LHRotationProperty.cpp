//
//  LHRotationProperty.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHRotationProperty.h"
#include "LHRotationFrame.h"
#include "LHDictionary.h"
#include "LHArray.h"

LHRotationProperty* LHRotationProperty::createWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    LHRotationProperty* aProp = new LHRotationProperty();
    
    if (aProp && aProp->initWithDictionary(dict, anim)){
        aProp->autorelease();
    }
    else{
        CC_SAFE_DELETE(aProp);
    }
    return aProp;
}
LHRotationProperty::LHRotationProperty(){
    
}
LHRotationProperty::~LHRotationProperty(){
    
}
bool LHRotationProperty::initWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    if(dict == NULL)return true; //special case for children subproperties
    
    if(LHAnimationProperty::initWithDictionary(dict, anim))
    {
        LHArray* framesInfo = dict->arrayForKey("Frames");
        if(framesInfo)
        {
            for(int i = 0; i < framesInfo->count(); ++i)
            {
                LHDictionary* frmInfo = framesInfo->dictAtIndex(i);
                
                LHRotationFrame* frm = LHRotationFrame::createWithDictionary(frmInfo, this);
                addKeyFrame(frm);
                
            }
        }
        return true;
    }
    return false;
}