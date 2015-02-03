//
//  LHUINode.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHUINode.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHDevice.h"

LHUINode::LHUINode()
{
}

LHUINode::~LHUINode()
{
    
}

LHUINode* LHUINode::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHUINode *ret = new LHUINode();
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

bool LHUINode::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        prnt->addChild(this);
        
        this->loadGenericInfoFromDictionary(dict);
        
        this->setPosition(Point(0,0));
        
#if COCOS2D_VERSION < 0x00030300
        this->setContentSize(prnt->getScene()->getContentSize());
#endif
        this->loadChildrenFromDictionary(dict);
        
        return true;
    }
    return false;
}
