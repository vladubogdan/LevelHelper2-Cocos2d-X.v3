//
//  LHFrame.cpp
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHFrame.h"
#include "LHDictionary.h"
#include "LHAnimationProperty.h"


LHFrame::~LHFrame()
{
    m_property = NULL;
}

LHFrame::LHFrame():m_property(NULL)
{
}

LHFrame* LHFrame::createWithDictionary(LHDictionary* dict, LHAnimationProperty* prop)
{
    LHFrame* frm = new LHFrame();
    
    if (frm && frm->initWithDictionary(dict, prop))
    {
        frm->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(frm);
    }
    
    return frm;
}

bool LHFrame::initWithDictionary(LHDictionary* dict, LHAnimationProperty* prop)
{
    m_property = prop;
    m_frameNumber = dict->intForKey("frameIndex");
    
    return true;
}