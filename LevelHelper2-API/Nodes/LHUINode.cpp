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
        
        loadGenericInfoFromDictionary(dict);
        
        //physics body needs to be created before adding this node to the parent
        loadPhysicsFromDictionary(dict->dictForKey("nodePhysics"), (LHScene*)prnt->getScene());
        
        prnt->addChild(this);
        
        this->setPosition(Point(0,0));
        this->setContentSize(prnt->getScene()->getContentSize());

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
        
        createAnimationsFromDictionary(dict);
        
        return true;
    }
    return false;
}

void LHUINode::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
{
    visitActiveAnimation();
    Node::visit(renderer, parentTransform, parentTransformUpdated);
}