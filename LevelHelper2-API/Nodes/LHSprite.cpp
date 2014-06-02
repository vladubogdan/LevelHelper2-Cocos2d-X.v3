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
    
    CCLOG("PLIST PATH %s", plistPath.c_str());
    
    if(Sprite::initWithSpriteFrameName(spriteName))
    {
        prnt->addChild(this);
        
        //some other initializing in the future
        return true;
    }
    return false;
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
        
        loadGenericInfoFromDictionary(dict);


        //physics body needs to be created before adding this node to the parent
        loadPhysicsFromDictionary(dict->dictForKey("nodePhysics"), (LHScene*)prnt->getScene());
        
        prnt->addChild(this);

        
        Point unitPos   = dict->pointForKey("generalPosition");
        Point pos       = LHScene::positionForNode(this, unitPos);
        
        LHDictionary* devPositions = dict->dictForKey("devicePositions");
        if(devPositions)
        {
            std::string unitPosStr = LHDevice::devicePosition(devPositions, LH_SCREEN_RESOLUTION);
            
            if(unitPosStr.length()>0){
                Point unitPos = PointFromString(unitPosStr);
                pos = LHScene::positionForNode(this, unitPos);
            }
        }

        
        this->setColor(dict->colorForKey("colorOverlay"));
        this->setOpacity(dict->floatForKey("alpha"));
        this->setRotation(dict->floatForKey("rotation"));
        this->setZOrder(dict->floatForKey("zOrder"));
        

        Point scl = dict->pointForKey("scale");
        this->setScaleX(scl.x);
        this->setScaleY(scl.y);
        
        
        Point anchor = dict->pointForKey("anchor");
        anchor.y = 1.0f - anchor.y;
        this->setAnchorPoint(anchor);
        
        this->setPosition(pos);
        
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

//void LHSprite::setOpacity(GLubyte opacity)
//{
//    Sprite::setOpacity(opacity);
//}
//
//void LHSprite::setRotation(float rotation)
//{
//    Sprite::setRotation(rotation);
//    
//    //*own stuff here*//
//}
//void LHSprite::setScaleX(float scaleX)
//{
//    Sprite::setScaleX(scaleX);
//}
//
//void LHSprite::setScaleY(float scaleY)
//{
//    Sprite::setScaleY(scaleY);
//}
//
//void LHSprite::setPosition(const Point& pos)
//{
//    Sprite::setPosition(pos);
//}
//
//Scene* LHSprite::getScene()
//{
//    return Sprite::getScene();
//}
void LHSprite::visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated)
{
    visitActiveAnimation();
    
    Sprite::visit(renderer, parentTransform, parentTransformUpdated);
}