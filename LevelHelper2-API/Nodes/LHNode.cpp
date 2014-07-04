//
//  LHNode.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHNode.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHDevice.h"

LHNode::LHNode()
{
}

LHNode::~LHNode()
{
    
}

LHNode* LHNode::createWithName(const std::string& nm)
{
    LHNode *ret = new LHNode();
    if (ret && ret->initWithName(nm))
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

LHNode* LHNode::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHNode *ret = new LHNode();
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
bool LHNode::initWithName(const std::string& nm)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        setName(nm);
        return true;
    }
    return false;
}
bool LHNode::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        prnt->addChild(this);
        
        this->loadGenericInfoFromDictionary(dict);
        this->loadTransformationInfoFromDictionary(dict);
        
        this->loadPhysicsFromDictionary(dict->dictForKey("nodePhysics"), (LHScene*)prnt->getScene());
        
        this->loadChildrenFromDictionary(dict);
        
        this->createAnimationsFromDictionary(dict);
        
        return true;
    }
    return false;
}

void LHNode::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
{
    visitPhysicsProtocol();
    visitActiveAnimation();
    Node::visit(renderer, parentTransform, parentTransformUpdated);
}


#if LH_USE_BOX2D
void LHNode::updatePosition(const cocos2d::Vec2 &pos){
    Node::setPosition(pos);
}
void LHNode::updateRotation(float rotation){
    Node::setRotation(rotation);
}
#endif
