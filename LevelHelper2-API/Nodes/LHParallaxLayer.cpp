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
}

LHParallaxLayer::~LHParallaxLayer()
{
    
}

LHParallaxLayer* LHParallaxLayer::parallaxLayerWithDictionary(LHDictionary* dict, Node* prnt)
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
        
        loadGenericInfoFromDictionary(dict);

        prnt->addChild(this);
        
        Point unitPos   = dict->pointForKey("generalPosition");
        Point pos       = LHScene::positionForNode(this, unitPos);
        
        LHDictionary* devPositions = dict->dictForKey("devicePositions");
        if(devPositions)
        {
            std::string unitPosStr = LHDevice::devicePosition(devPositions, LH_SCREEN_RESOLUTION);
            
            if(unitPosStr.length()>0){
                Point unitPos = PointFromString(unitPosStr);
                pos = LHScene::positionForNode(this, unitPos);
            }
        }
        
        this->setZOrder(dict->floatForKey("zOrder"));
        
        _xRatio = dict->floatForKey("xRatio");
        _yRatio = dict->floatForKey("yRatio");

        LHArray* childrenInfo = dict->arrayForKey("children");
        if(childrenInfo)
        {
            for(int i = 0; i < childrenInfo->count(); ++i)
            {
                LHDictionary* childInfo = childrenInfo->dictAtIndex(i);
                
                Node* node = LHScene::createLHNodeWithDictionary(childInfo, this);
#pragma unused (node)
            }
        }
        
        this->setPosition(pos);
        
        return true;
    }
    
    return false;
}
