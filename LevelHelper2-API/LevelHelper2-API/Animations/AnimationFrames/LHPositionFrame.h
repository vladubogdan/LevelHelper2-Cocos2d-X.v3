//
//  LHPositionFrame.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_ANIM_POSITION_FRAME_H__
#define __LEVELHELPER_API_ANIM_POSITION_FRAME_H__

#include "LHFrame.h"

class LHPositionFrame : public LHFrame
{
public:
    
    static LHPositionFrame* createWithDictionary(LHDictionary* dict, LHAnimationProperty* prop);
    
    LHPositionFrame();
    virtual ~LHPositionFrame();
    virtual bool initWithDictionary(LHDictionary* dict, LHAnimationProperty* prop);
    
    
    Point positionForUUID(const std::string& uuid);
    
private:
    std::map<std::string, Point> _positions;
};

#endif //__LEVELHELPER_API_ANIM_POSITION_FRAME_H__