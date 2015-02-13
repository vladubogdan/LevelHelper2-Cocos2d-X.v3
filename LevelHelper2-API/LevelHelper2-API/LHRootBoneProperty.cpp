//
//  LHRootBoneProperty.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHRootBoneProperty.h"
#include "LHBoneFrame.h"
#include "LHDictionary.h"
#include "LHArray.h"


LHRootBoneProperty* LHRootBoneProperty::createWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    LHRootBoneProperty* aProp = new LHRootBoneProperty();
    
    if (aProp && aProp->initWithDictionary(dict, anim)){
        aProp->autorelease();
    }
    else{
        CC_SAFE_DELETE(aProp);
    }
    return aProp;
}
LHRootBoneProperty::LHRootBoneProperty(){
    
}
LHRootBoneProperty::~LHRootBoneProperty(){
    
}
bool LHRootBoneProperty::initWithDictionary(LHDictionary* dict, LHAnimation* anim)
{
    if(dict == NULL)return true; //special case for children subproperties
    
    if(LHAnimationProperty::initWithDictionary(dict, anim))
    {
        LHArray* framesInfo = dict->arrayForKey("Frames");
        if(framesInfo){
            for(int i = 0; i < framesInfo->count(); ++i)
            {
                LHDictionary* frmInfo = framesInfo->dictAtIndex(i);

                LHBoneFrame* frm = LHBoneFrame::createWithDictionary(frmInfo, this);
                addKeyFrame(frm);
            }
        }
        return true;
    }
    return false;
}