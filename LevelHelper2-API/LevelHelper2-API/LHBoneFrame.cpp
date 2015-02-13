//
//  LHBoneFrame.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHBoneFrame.h"
#include "LHDictionary.h"
#include "LHAnimationProperty.h"

//#include "LHAnimation.h"

LHBoneFrameInfo::LHBoneFrameInfo(LHDictionary* dict)
{
    position = dict->pointForKey("pos");
    rotation = dict->floatForKey("rot");
}

////////////////////////////////////////////////////////////////////////////////


LHBoneFrame* LHBoneFrame::createWithDictionary(LHDictionary* dict,
                                               LHAnimationProperty* prop)
{
    LHBoneFrame* frm = new LHBoneFrame();
    if (frm && frm->initWithDictionary(dict, prop)){
        frm->autorelease();
    }
    else{
        CC_SAFE_DELETE(frm);
    }
    return frm;
}

LHBoneFrame::LHBoneFrame()
{

}

LHBoneFrame::~LHBoneFrame()
{
    for (auto iter = bonesInfo.begin(); iter != bonesInfo.end(); ++iter)
    {
        LHBoneFrameInfo* obj = iter->second;
        delete obj;
    }
    bonesInfo.clear();
}

bool LHBoneFrame::initWithDictionary(LHDictionary* dict, LHAnimationProperty* prop)
{
    if(LHFrame::initWithDictionary(dict, prop))
    {
        LHDictionary* savedBonesInfo = dict->dictForKey("bonesInfo");
        if(savedBonesInfo)
        {
            __Array* allKeys = savedBonesInfo->allKeys();
            if(allKeys)
            {
                for(int i = 0; i < allKeys->count();++i)
                {
                    std::string nm = ((__String*)allKeys->getObjectAtIndex(i))->getCString();

                    LHDictionary* boneDict = savedBonesInfo->dictForKey(nm);

                    LHBoneFrameInfo* inf = new LHBoneFrameInfo(boneDict);
                    
                    bonesInfo[nm] = inf;
                }
            }
        }
        return true;
    }
    
    return false;
}

LHBoneFrameInfo* LHBoneFrame::boneFrameInfoForBoneNamed(const std::string& nm){
    return bonesInfo[nm];
}
