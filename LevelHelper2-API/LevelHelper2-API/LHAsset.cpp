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
#include "LHConfig.h"
#include "LHGameWorldNode.h"

LHAsset::LHAsset()
{
    _tracedFixtures = nullptr;
}

LHAsset::~LHAsset()
{
    this->shouldRemoveBody();
    CC_SAFE_RELEASE(_tracedFixtures);
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
        
        LHGameWorldNode* gwNode = scene->getGameWorldNode();
        float oldScale = gwNode->getScale();
        Point oldPos = gwNode->getPosition();
        gwNode->setScale(1.0f);
        gwNode->setPosition(Point(0,0));
        
        
        LHDictionary* assetInfo = (LHDictionary*)scene->assetInfoForFile(assetFileName);
        if(assetInfo)
        {
            LHDictionary* tracedFixInfo = assetInfo->dictForKey("tracedFixtures");
            if(tracedFixInfo){
                _tracedFixtures = __Dictionary::createWithDictionary(tracedFixInfo);
                _tracedFixtures->retain();
            }
            
            prnt->addChild(this);
            this->loadChildrenFromDictionary(assetInfo);
        }
        
        gwNode->setScale(oldScale);
        gwNode->setPosition(oldPos);
        
        return true;
    }
    return false;
}

LHAsset* LHAsset::nodeWithDictionary(LHDictionary* dict, Node* prnt)
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
        
        LHGameWorldNode* gwNode = scene->getGameWorldNode();
        float oldScale = gwNode->getScale();
        Point oldPos = gwNode->getPosition();
        gwNode->setScale(1.0f);
        gwNode->setPosition(Point(0,0));
        
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
        
        
        bool foundFile = false;
        if(dict->objectForKey("assetFile"))
        {
            LHDictionary* assetInfo = (LHDictionary*)scene->assetInfoForFile(dict->stringForKey("assetFile"));
            if(assetInfo){
                
                LHDictionary* tracedFixInfo = assetInfo->dictForKey("tracedFixtures");
                if(tracedFixInfo){
                    _tracedFixtures = __Dictionary::createWithDictionary(tracedFixInfo);
                    _tracedFixtures->retain();
                }

                foundFile = true;
                this->loadChildrenFromDictionary(assetInfo);
            }
        }
        if(!foundFile){
            CCLOG("WARNING: COULD NOT FIND INFORMATION FOR ASSET %s. This usually means that the asset was created but not saved. Check your level and in the Scene Navigator, click on the lock icon next to the asset name.", this->getName().c_str());
            this->loadChildrenFromDictionary(dict);
        }

        this->createAnimationsFromDictionary(dict);
        
        gwNode->setScale(oldScale);
        gwNode->setPosition(oldPos);
        
        return true;
    }
    return false;
}
__Array* LHAsset::tracedFixturesWithUUID(const std::string& uuid)
{
    if(!_tracedFixtures)return nullptr;
    return (__Array*)_tracedFixtures->objectForKey(uuid);
}


#if COCOS2D_VERSION >= 0x00030200
void LHAsset::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
#else
void LHAsset::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
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
void LHAsset::setPosition(const cocos2d::Vec2 &pos){
    Node::setPosition(pos);
    this->updatePhysicsTransform();
    for (auto& child : this->getChildren()){
        child->setPosition(child->getPosition());
    }
}
void LHAsset::setRotation(float rotation){
    Node::setRotation(rotation);
    this->updatePhysicsTransform();
    for (auto& child : this->getChildren()){
        child->setRotation(child->getRotation());
    }
}
void LHAsset::setScaleX(float scaleX){
    
    Node::setScaleX(scaleX);
    this->updatePhysicsScale();
    for (auto& child : this->getChildren()){
        child->setScaleX(child->getScaleX());
    }
}
void LHAsset::setScaleY(float scaleY){
    Node::setScaleY(scaleY);
    this->updatePhysicsScale();
    for (auto& child : this->getChildren()){
        child->setScaleY(child->getScaleY());
    }
}
void LHAsset::setScale(float scaleX, float scaleY){
    Node::setScale(scaleX, scaleY);
    this->updatePhysicsScale();
    for (auto& child : this->getChildren()){
        child->setScaleX(child->getScaleX());
    }
}
void LHAsset::updatePosition(const cocos2d::Vec2 &pos){
    Node::setPosition(pos);
}
void LHAsset::updateRotation(float rotation){
    Node::setRotation(rotation);
}
#endif


