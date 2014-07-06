//
//  LHAsset.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHAsset.h"
#include "LHUtils.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHDevice.h"

LHAsset::LHAsset()
{
}

LHAsset::~LHAsset()
{
    
}

LHAsset* LHAsset::createWithName(const std::string& nm, const std::string& assetFileName, Node* prnt)
{
    LHAsset *ret = new LHAsset();
    if (ret && ret->initWithName(nm, assetFileName, prnt))
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

bool LHAsset::initWithName(const std::string& nm, const std::string& assetFileName, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        setName(nm);
        LHScene* scene = (LHScene*)prnt->getScene();
        
        LHDictionary* assetInfo = (LHDictionary*)scene->assetInfoForFile(assetFileName);
        if(assetInfo)
        {
            prnt->addChild(this);
            this->loadChildrenFromDictionary(assetInfo);
        }
        
        return true;
    }
    return false;
}

LHAsset* LHAsset::assetNodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHAsset *ret = new LHAsset();
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

bool LHAsset::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        
        LHScene* scene = (LHScene*)prnt->getScene();
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
        
        
        if(dict->objectForKey("assetFile"))
        {
            LHDictionary* assetInfo = (LHDictionary*)scene->assetInfoForFile(dict->stringForKey("assetFile"));
            if(assetInfo){
                this->loadChildrenFromDictionary(assetInfo);
            }
            else{
                CCLOG("WARNING: COULD NOT FIND INFORMATION FOR ASSET %s.", this->getName().c_str());
            }
        }

        this->createAnimationsFromDictionary(dict);
        
        return true;
    }
    return false;
}

void LHAsset::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
{
    visitPhysicsProtocol();
    visitActiveAnimation();
    Node::visit(renderer, parentTransform, parentTransformUpdated);
}


#if LH_USE_BOX2D
void LHAsset::removeFromParent(){
    this->removeBody();
    Node::removeFromParent();
}
void LHAsset::setPosition(const cocos2d::Vec2 &pos){
    Node::setPosition(pos);
    this->updatePhysicsTransform();
}
void LHAsset::setRotation(float rotation){
    Node::setRotation(rotation);
    this->updatePhysicsTransform();
}
void LHAsset::setScaleX(float scaleX){
    Node::setScaleX(scaleX);
    this->updatePhysicsScale();
}
void LHAsset::setScaleY(float scaleY){
    Node::setScaleY(scaleY);
    this->updatePhysicsScale();
}
void LHAsset::setScale(float scaleX, float scaleY){
    Node::setScale(scaleX, scaleY);
    this->updatePhysicsScale();
}
void LHAsset::updatePosition(const cocos2d::Vec2 &pos){
    Node::setPosition(pos);
}
void LHAsset::updateRotation(float rotation){
    Node::setRotation(rotation);
}
#endif


