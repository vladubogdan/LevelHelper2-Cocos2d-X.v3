//
//  LHParallaxLayer.m
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHParallaxLayer.h"
#include "LHUtils.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHParallax.h"
#include "LHDevice.h"

LHParallaxLayer::LHParallaxLayer()
{
    initialPosition = Point();
}

LHParallaxLayer::~LHParallaxLayer()
{
    
}

LHParallaxLayer* LHParallaxLayer::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHParallaxLayer *ret = new LHParallaxLayer();
    if (ret && ret->initWithDictionary(dict, prnt))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool LHParallaxLayer::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        prnt->addChild(this);
        
        this->loadGenericInfoFromDictionary(dict);

        this->loadTransformationInfoFromDictionary(dict);
        
        _xRatio = dict->floatForKey("xRatio");
        _yRatio = dict->floatForKey("yRatio");

        this->loadChildrenFromDictionary(dict);

        initialPosition = this->getPosition();
        
        return true;
    }
    
    return false;
}
