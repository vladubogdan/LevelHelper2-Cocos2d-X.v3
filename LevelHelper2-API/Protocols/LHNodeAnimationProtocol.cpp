//
//  LHNodeProtocol.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 24/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHNodeAnimationProtocol.h"
#include "LHDictionary.h"
#include "LHArray.h"
#include "LHAnimation.h"
#include "LHSprite.h"

LHNodeAnimationProtocol::LHNodeAnimationProtocol():_animations(NULL), _activeAnimation(NULL)
{
}

LHNodeAnimationProtocol::~LHNodeAnimationProtocol()
{
    _activeAnimation = NULL;
    CC_SAFE_DELETE(_animations);
}

void LHNodeAnimationProtocol::setActiveAnimation(LHAnimation* animation)
{
    _activeAnimation = animation;
}


void LHNodeAnimationProtocol::createAnimationsFromDictionary(LHDictionary* dict)
{
    if(dict->objectForKey("animations"))
    {
        LHArray* animsInfo = dict->arrayForKey("animations");
        for(int i = 0; i < animsInfo->count(); ++i)
        {
            LHDictionary* animInf = animsInfo->dictAtIndex(i);

            if(!_animations){
                _animations = new __Array();
                _animations->init();
            }
            LHAnimation* animation = LHAnimation::createWithDictionary(animInf, this);
            if(animation->isActive()){
                _activeAnimation = animation;
            }
            _animations->addObject(animation);
        }
    }
}

void LHNodeAnimationProtocol::visitActiveAnimation()
{
    float dt = Director::getInstance()->getDeltaTime();
    if(_activeAnimation){
        _activeAnimation->updateTimeWithDelta(dt);
    }
}
