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
#include "LHConfig.h"

LHNode::LHNode()
{
}

LHNode::~LHNode()
{
    this->shouldRemoveBody();
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
        this->loadGenericInfoFromDictionary(dict);
        
#if LH_USE_BOX2D
        prnt->addChild(this);
        this->loadTransformationInfoFromDictionary(dict);
        this->loadPhysicsFromDictionary(dict->dictForKey("nodePhysics"), (LHScene*)prnt->getScene());
#else
        //cocos2d-chipmunk required that the body is loaded before adding the node to the parent
        this->loadPhysicsFromDictionary(dict->dictForKey("nodePhysics"), (LHScene*)prnt->getScene());
        prnt->addChild(this);
        this->loadTransformationInfoFromDictionary(dict);
#endif

        this->loadChildrenFromDictionary(dict);
        this->createAnimationsFromDictionary(dict);
        
        return true;
    }
    return false;
}

#if COCOS2D_VERSION >= 0x00030200
void LHNode::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
#else
void LHNode::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
#endif
{
    visitPhysicsProtocol();
    visitActiveAnimation();

    if(renderer)
    {
#if COCOS2D_VERSION >= 0x00030200
        Node::visit(renderer, parentTransform, parentFlags);
#else
        Node::visit(renderer, parentTransform, parentTransformUpdated);
#endif
    }
}


#if LH_USE_BOX2D
void LHNode::setPosition(const cocos2d::Vec2 &pos){
    Node::setPosition(pos);
    this->updatePhysicsTransform();
    for (auto& child : this->getChildren()){
        child->setPosition(child->getPosition());
    }
}
void LHNode::setRotation(float rotation){
    Node::setRotation(rotation);
    this->updatePhysicsTransform();
    for (auto& child : this->getChildren()){
        child->setRotation(child->getRotation());
    }
}
void LHNode::setScaleX(float scaleX){    
    Node::setScaleX(scaleX);
    this->updatePhysicsScale();
    for (auto& child : this->getChildren()){
        child->setScaleX(child->getScaleX());
    }
}
void LHNode::setScaleY(float scaleY){
    Node::setScaleY(scaleY);
    this->updatePhysicsScale();
    for (auto& child : this->getChildren()){
        child->setScaleY(child->getScaleY());
    }
}
void LHNode::setScale(float scaleX, float scaleY){
    Node::setScale(scaleX, scaleY);
    this->updatePhysicsScale();
    for (auto& child : this->getChildren()){
        child->setScaleX(child->getScaleX());
    }
}
void LHNode::updatePosition(const cocos2d::Vec2 &pos){
    Node::setPosition(pos);
}
void LHNode::updateRotation(float rotation){
    Node::setRotation(rotation);
}
#endif

