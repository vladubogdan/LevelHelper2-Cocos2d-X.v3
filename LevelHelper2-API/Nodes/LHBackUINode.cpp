//
//  LHBackUINode.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHBackUINode.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHDevice.h"

LHBackUINode::LHBackUINode()
{
}

LHBackUINode::~LHBackUINode()
{
    
}

LHBackUINode* LHBackUINode::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHBackUINode *ret = new LHBackUINode();
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

bool LHBackUINode::initWithDictionary(LHDictionary* dict, Node* prnt)
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

void LHBackUINode::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
{
    visitActiveAnimation();
    Node::visit(renderer, parentTransform, parentTransformUpdated);
}