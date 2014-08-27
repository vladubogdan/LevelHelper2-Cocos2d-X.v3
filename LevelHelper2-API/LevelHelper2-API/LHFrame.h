//
//  LHFrame.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_ANIM_FRAME_H__
#define __LEVELHELPER_API_ANIM_FRAME_H__


#include "cocos2d.h"

USING_NS_CC;

class LHAnimationProperty;
class LHDictionary;

class LHFrame : public Ref
{
public:
   
    static LHFrame* createWithDictionary(LHDictionary* dict, LHAnimationProperty* prop);
    LHFrame();
    virtual ~LHFrame();
    
    virtual bool initWithDictionary(LHDictionary* dict, LHAnimationProperty* prop);
    
    void setWasShot(bool val){m_wasShot = val;}
    bool wasShot(){return m_wasShot;}
    
    int frameNumber(){return m_frameNumber;}
    LHAnimationProperty* property(){return m_property;}
private:
    int     m_frameNumber;
    bool    m_wasShot;
    LHAnimationProperty* m_property;
};

#endif // __LEVELHELPER_API_ANIM_FRAME_H__