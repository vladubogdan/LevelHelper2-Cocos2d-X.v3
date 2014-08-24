//
//  LHScaleFrame.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_ANIM_SCALE_FRAME_H__
#define __LEVELHELPER_API_ANIM_SCALE_FRAME_H__

#include "LHFrame.h"

class LHScaleFrame : public LHFrame
{
public:
 
    static LHScaleFrame* createWithDictionary(LHDictionary* dict, LHAnimationProperty* prop);
    
    LHScaleFrame();
    virtual ~LHScaleFrame();
    virtual bool initWithDictionary(LHDictionary* dict, LHAnimationProperty* prop);
    
    
    Size scaleForUUID(const std::string& uuid);
    
private:
    std::map<std::string, Size> _scales;
};

#endif //__LEVELHELPER_API_ANIM_SCALE_FRAME_H__
