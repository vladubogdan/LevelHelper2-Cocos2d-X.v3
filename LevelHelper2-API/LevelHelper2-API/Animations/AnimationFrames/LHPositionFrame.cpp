//
//  LHPositionFrame.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHPositionFrame.h"
#include "LHDictionary.h"
#include "LHAnimationProperty.h"

//#include "LHAnimation.h"

LHPositionFrame* LHPositionFrame::createWithDictionary(LHDictionary* dict,
                                                       LHAnimationProperty* prop)
{
    LHPositionFrame* frm = new LHPositionFrame();
    if (frm && frm->initWithDictionary(dict, prop)){
        frm->autorelease();
    }
    else{
        CC_SAFE_DELETE(frm);
    }
    return frm;
}

LHPositionFrame::LHPositionFrame()
{

}
LHPositionFrame::~LHPositionFrame()
{
    
}
bool LHPositionFrame::initWithDictionary(LHDictionary* dict, LHAnimationProperty* prop)
{
    if(LHFrame::initWithDictionary(dict, prop))
    {
        LHDictionary* positionsInfo = dict->dictForKey("positions");
        if(positionsInfo)
        {
            __Array* allKeys = positionsInfo->allKeys();
            if(allKeys)
            {
                for(int i = 0; i < allKeys->count();++i)
                {
                    std::string uuid = ((__String*)allKeys->getObjectAtIndex(i))->getCString();

                    Point pos = positionsInfo->pointForKey(uuid);

                    _positions[uuid] = pos;
                }
            }
        }
        return true;
    }
    
    return false;
}

Point LHPositionFrame::positionForUUID(const std::string& uuid){
    return _positions[uuid];
}
