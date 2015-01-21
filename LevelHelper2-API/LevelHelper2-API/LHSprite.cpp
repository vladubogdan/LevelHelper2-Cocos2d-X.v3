//
//  LHSprite.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 24/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHSprite.h"
#include "LHUtils.h"
#include "LHScene.h"
#include "LHDictionary.h"
#include "LHArray.h"
#include "LHDevice.h"

LHSprite::LHSprite()
{
}

LHSprite::~LHSprite()
{
    this->shouldRemoveBody();    
}

void LHSprite::cacheSpriteFramesInfo(const std::string& imageFilePath, LHScene* scene)
{
    std::string plist_path = imageFilePath;
    size_t extPos = plist_path.rfind('.');
    if (extPos != std::string::npos)
    {
        // Erase the current extension.
        plist_path.erase(extPos);
        // Add the new extension.
        plist_path.append(".plist");
    }
    
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(imageFilePath);
    if (texture)
    {
        std::string fullPlistPath = FileUtils::getInstance()->fullPathForFilename(plist_path);

#if COCOS2D_VERSION > 0x00030200
        
        std::string plist_content = FileUtils::getInstance()->getStringFromFile(fullPlistPath);
        SpriteFrameCache::getInstance()->addSpriteFramesWithFileContent(plist_content, texture);
        ValueMap dictionary = FileUtils::getInstance()->getValueMapFromData(plist_content.c_str(), static_cast<int>(plist_content.size()));
#else
        
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(fullPlistPath);
        ValueMap dictionary = FileUtils::getInstance()->getValueMapFromFile(fullPlistPath);
#endif
        
        ValueMap& framesDict = dictionary["frames"].asValueMap();
        
        //here we add all bodies info found in this plist file
        //I do it this way in order to avoid loading the file again
        //this way - we only read this plist file once
        for (auto iter = framesDict.begin(); iter != framesDict.end(); ++iter)
        {
            ValueMap& frameDict = iter->second.asValueMap();
            std::string sprFrameName = iter->first;
            Value& bodyInfo = frameDict["body"];
            scene->setEditorBodyInfoForSpriteName(sprFrameName, imageFilePath, bodyInfo);
        }
    }
}

LHSprite* LHSprite::createWithFile(const std::string& filename, const std::string& folder, Node* prnt)
{
    LHSprite *ret = new LHSprite();
    if (ret && ret->initWithFilename(filename, folder, prnt))
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
bool LHSprite::initWithFilename(const std::string& filename, const std::string& folder, Node* prnt)
{
    LHScene* scene = (LHScene*)prnt->getScene();
    
    std::string imagePath = LHUtils::getImagePathWithFilename(filename,
                                                              folder,
                                                              scene->getCurrentDeviceSuffix());
    if(Sprite::initWithFile(imagePath))
    {
        prnt->addChild(this);

        //some other initializing in the future
        return true;
    }
    return false;
}

LHSprite* LHSprite::createWithSpriteName(const std::string& spriteFrameName,
                                         const std::string& imageFile,
                                         const std::string& folder,
                                         Node* prnt)
{
    LHSprite *ret = new LHSprite();
    if (ret && ret->initWithSpriteName(spriteFrameName, imageFile, folder, prnt))
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
bool LHSprite::initWithSpriteName(const std::string& spriteName,
                                       const std::string& imageFile,
                                       const std::string& folder,
                                       Node* prnt)
{
    LHScene* scene = (LHScene*)prnt->getScene();
    
    std::string imagePath = LHUtils::getImagePathWithFilename(imageFile,
                                                              folder,
                                                              scene->getCurrentDeviceSuffix());
    
    this->cacheSpriteFramesInfo(imagePath, scene);
    
    if(Sprite::initWithSpriteFrameName(spriteName))
    {
        this->setName(spriteName);
        spriteFrameName = spriteName;
        imageFilePath = imagePath;
        
        prnt->addChild(this);
        
        Value* phyInfo = scene->getEditorBodyInfoForSpriteName(spriteName, imagePath);
        ValueMap phyDict = phyInfo->asValueMap();
        
        this->loadPhysicsFromValueMap(phyDict, scene);
        
        //some other initializing in the future
        return true;
    }
    return false;
}



LHSprite* LHSprite::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHSprite *ret = new LHSprite();
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


bool LHSprite::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHScene* scene = (LHScene*)prnt->getScene();
    
    std::string imageFile = dict->stringForKey("imageFileName");
    std::string relativeImgPath = dict->stringForKey("relativeImagePath");
    
    imageFilePath = LHUtils::getImagePathWithFilename(imageFile,
                                                      relativeImgPath,
                                                      scene->getCurrentDeviceSuffix());

    bool initResult = false;
    

    if(dict->objectForKey("spriteName"))
    {
        spriteFrameName = dict->stringForKey("spriteName");
        
        if(NULL == SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName) ||
           false == scene->hasEditorBodyInfoForImageFilePath(imageFilePath))
        {
            //dont reload if already loaded
            this->cacheSpriteFramesInfo(imageFilePath, scene);
            //we are no longer using this method because we need to load the physics info if available
            //SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist_path, imagePath);
        }
        
        initResult = initWithSpriteFrameName(spriteFrameName);
    }
    else{
        initResult = initWithFile(imageFilePath);
    }
    
    if(initResult)
    {
        
        _physicsBody = NULL;
        Size curSize = this->getContentSize();
        this->loadGenericInfoFromDictionary(dict);
        this->setContentSize(curSize);
        
#if LH_USE_BOX2D
        prnt->addChild(this);
        this->loadTransformationInfoFromDictionary(dict);
        this->loadPhysicsFromDictionary(dict->dictForKey("nodePhysics"), scene);
#else
        
        //cocos2d-chipmunk required that the body is loaded before adding the node to the parent
        this->loadPhysicsFromDictionary(dict->dictForKey("nodePhysics"), scene);
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
void LHSprite::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
#else
void LHSprite::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
#endif
{
    visitPhysicsProtocol();
    visitActiveAnimation();

    if(renderer)
    {
#if COCOS2D_VERSION >= 0x00030200
        Sprite::visit(renderer, parentTransform, parentFlags);
#else
        Sprite::visit(renderer, parentTransform, parentTransformUpdated);
#endif
    }
}

#if LH_USE_BOX2D

void LHSprite::setPosition(const cocos2d::Vec2 &pos)
{
    Sprite::setPosition(pos);
    this->updatePhysicsTransform();
    
    for (auto& child : this->getChildren()){
        child->setPosition(child->getPosition());
    }
}

void LHSprite::setRotation(float rotation)
{
    Sprite::setRotation(rotation);
    this->updatePhysicsTransform();
    for (auto& child : this->getChildren()){
        child->setRotation(child->getRotation());
    }
}

void LHSprite::setScaleX(float scaleX){

    Sprite::setScaleX(scaleX);
    this->updatePhysicsScale();
    for (auto& child : this->getChildren()){
        child->setScaleX(child->getScaleX());
    }
}
void LHSprite::setScaleY(float scaleY){
    Sprite::setScaleY(scaleY);
    this->updatePhysicsScale();
    for (auto& child : this->getChildren()){
        child->setScaleY(child->getScaleY());
    }
}

void LHSprite::setScale(float scaleX, float scaleY){
    Sprite::setScale(scaleX, scaleY);
    this->updatePhysicsScale();
    for (auto& child : this->getChildren()){
        child->setScaleX(child->getScaleX());
    }
}

void LHSprite::updatePosition(const cocos2d::Vec2 &pos){
    Sprite::setPosition(pos);
}
void LHSprite::updateRotation(float rotation){
    Sprite::setRotation(rotation);
}
#endif

