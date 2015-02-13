//
//  LHBoneNodes.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHBoneNodes.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHDevice.h"
#include "LHConfig.h"

LHBoneNodes::LHBoneNodes()
{
}

LHBoneNodes::~LHBoneNodes()
{
}

LHBoneNodes* LHBoneNodes::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHBoneNodes *ret = new LHBoneNodes();
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

bool LHBoneNodes::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        this->loadGenericInfoFromDictionary(dict);
        
        prnt->addChild(this);
        this->loadTransformationInfoFromDictionary(dict);

        this->loadChildrenFromDictionary(dict);
        
        return true;
    }
    return false;
}
