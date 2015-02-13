//
//  LHBoneFrame.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_ANIM_BONE_FRAME_H__
#define __LEVELHELPER_API_ANIM_BONE_FRAME_H__

#include "LHFrame.h"

class LHBoneFrameInfo
{
public:
    LHBoneFrameInfo(LHDictionary* dict);
    virtual ~LHBoneFrameInfo(){};
    
    Point position;
    float rotation;
};


class LHBoneFrame : public LHFrame
{
public:
    
    static LHBoneFrame* createWithDictionary(LHDictionary* dict, LHAnimationProperty* prop);
    
    LHBoneFrame();
    virtual ~LHBoneFrame();
    virtual bool initWithDictionary(LHDictionary* dict, LHAnimationProperty* prop);
    
    
    LHBoneFrameInfo* boneFrameInfoForBoneNamed(const std::string& nm);
    
private:
    std::map<std::string, LHBoneFrameInfo*> bonesInfo;
};

#endif //__LEVELHELPER_API_ANIM_BONE_FRAME_H__