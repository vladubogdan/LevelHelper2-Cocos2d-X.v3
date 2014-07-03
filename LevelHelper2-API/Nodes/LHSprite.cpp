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
                                         const std::string& plistFile,
                                         const std::string& plistFolder,
                                         Node* prnt)
{
    LHSprite *ret = new LHSprite();
    if (ret && ret->initWithSpriteName(spriteFrameName, plistFile, plistFolder, prnt))
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
                                  const std::string& plistFile,
                                  const std::string& plistFolder,
                                  Node* prnt)
{
    LHScene* scene = (LHScene*)prnt->getScene();
    
    std::string plistPath = LHUtils::getImagePathWithFilename(plistFile,
                                                              plistFolder,
                                                              scene->getCurrentDeviceSuffix());
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistPath);
    
    if(Sprite::initWithSpriteFrameName(spriteName))
    {
        prnt->addChild(this);
        
        //some other initializing in the future
        return true;
    }
    return false;
}



LHSprite* LHSprite::spriteNodeWithDictionary(LHDictionary* dict, Node* prnt)
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
    
    std::string imagePath = LHUtils::getImagePathWithFilename(imageFile,
                                                              relativeImgPath,
                                                              scene->getCurrentDeviceSuffix());

    bool initResult = false;
    
    if(dict->objectForKey("spriteName"))
    {
        std::string spriteName = dict->stringForKey("spriteName");
        
        std::string plist_path = imagePath;
        size_t extPos = plist_path.rfind('.');
        if (extPos != std::string::npos)
        {
            // Erase the current extension.
            plist_path.erase(extPos);
            // Add the new extension.
            plist_path.append(".plist");
        }
        
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist_path, imagePath);
        
        initResult = initWithSpriteFrameName(spriteName);
    }
    else{
        initResult = initWithFile(imagePath);
    }
    
    if(initResult)
    {
        _physicsBody = NULL;

#if LH_USE_BOX2D == 0
        //cocos2d/chipmunk required that we load the body prior adding the sprite to the parent
        this->loadPhysicsFromDictionary(dict->dictForKey("nodePhysics"), (LHScene*)prnt->getScene());
#endif
        
        prnt->addChild(this);
        Size curSize = this->getContentSize();
        this->loadGenericInfoFromDictionary(dict);
        this->loadTransformationInfoFromDictionary(dict);
        this->setContentSize(curSize);
        
#if LH_USE_BOX2D
        this->loadPhysicsFromDictionary(dict->dictForKey("nodePhysics"), (LHScene*)prnt->getScene());
#endif

        
        this->loadChildrenFromDictionary(dict);
        this->createAnimationsFromDictionary(dict);
        
        return true;
    }
        

    return false;
}

void LHSprite::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
{
    visitNodeProtocol();
    visitActiveAnimation();
    Sprite::visit(renderer, parentTransform, parentTransformUpdated);
}


