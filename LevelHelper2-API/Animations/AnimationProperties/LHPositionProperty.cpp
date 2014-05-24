//
//  LHPositionProperty.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHPositionProperty.h"
#include "LHPositionFrame.h"
#include "LHDictionary.h"
#include "LHArray.h"


LHPositionProperty* LHPositionProperty::createWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    LHPositionProperty* aProp = new LHPositionProperty();
    
    if (aProp && aProp->initWithDictionary(dict, anim)){
        aProp->autorelease();
    }
    else{
        CC_SAFE_DELETE(aProp);
    }
    return aProp;
}
LHPositionProperty::LHPositionProperty(){
    
}
LHPositionProperty::~LHPositionProperty(){
    
}
bool LHPositionProperty::initWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    if(dict == NULL)return true; //special case for children subproperties
    
    if(LHAnimationProperty::initWithDictionary(dict, anim))
    {
        LHArray* framesInfo = dict->arrayForKey("Frames");
        if(framesInfo){
            for(int i = 0; i < framesInfo->count(); ++i)
            {
                LHDictionary* frmInfo = framesInfo->dictAtIndex(i);

                LHPositionFrame* frm = LHPositionFrame::createWithDictionary(frmInfo, this);
                addKeyFrame(frm);
            }
        }
        return true;
    }
    return false;
}