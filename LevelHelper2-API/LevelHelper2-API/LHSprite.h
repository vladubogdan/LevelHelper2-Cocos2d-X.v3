//
//  LHSprite.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 24/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_SPRITE_H__
#define __LEVELHELPER_API_SPRITE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHNodeAnimationProtocol.h"
#include "LHPhysicsProtocol.h"

using namespace cocos2d;

/**
 LHSprite class is used to load textured rectangles that are found in a level file.
 Users can retrieve a sprite object by calling the scene (LHScene) childNodeWithName: method.
 */

class LHDictionary;
class LHScene;

class LHSprite : public Sprite, public LHNodeProtocol, public LHNodeAnimationProtocol, public LHPhysicsProtocol
{

public:

    /**
     * Creates a sprite with an image filename.
     *
     * @param   filename A path to image file, e.g., "scene1/monster.png"
     * @param   folder The folder containing the Cocos2d-x suffix folders. The folder must be added as reference in Xcode. If the suffix folders are added directly in the project this parammeter should be "".
     * @param   prnt A parent node. Must be in the LHScene hierarchy.
     * @return  An autoreleased sprite object.
     Eg:
     @code
     INDIVIDUAL_IMAGES_FOLDER/ (added as reference in Xcode - has Blue icon)
        568/
            background.png
        hd/
            background.png
        ipad/
            background.png
        ipadhd/
            background.png
     
     LHSprite* spr = LHSprite::createWithFile("background.png", "INDIVIDUAL_IMAGES_FOLDER/", this);
     if(spr){
        spr->setAnchorPoint(Point(0,0));
        spr->setPosition(Point(0,0));
        spr->setLocalZOrder(-4);
     }
     @endcode
     */
    static LHSprite* createWithFile(const std::string& filename, const std::string& folder, Node* prnt);
    
    /**
     * Creates a sprite with an sprite frame name using a sprite sheet name.
     *
     * @param   spriteName A null terminated string which indicates the sprite frame name.
     * @param   imageFile A null terminated string which indicates the image file containing the sprite texture. This file will be used to look for the plist file.
     * @param   folder A null terminated string which indicates the folder that contains the image & plist file. The folder must be added as reference in Xcode - blue icon.
     * @param   prnt A parent node. Must be in the LHScene hierarchy.
     * @return  An autoreleased sprite object
     
     Eg:
     @code
     DEMO_PUBLISH_FOLDER/ (added as reference in Xcode - has Blue icon)
        568/
            objects.plist
            objects.png
        hd/
            objects.plist
            objects.png
        ipad/
            objects.plist
            objects.png
        ipadhd/
            objects.plist
            objects.png
     
     LHSprite* spr = LHSprite::createWithSpriteName("object_backpack", "objects.png", "DEMO_PUBLISH_FOLDER/", this->getGameWorld());
     if(spr){
        spr->setPosition(Point(400,350));
     }
     @endcode
     */
    static LHSprite* createWithSpriteName(const std::string& spriteName,
                                          const std::string& imageFile,
                                          const std::string& folder,
                                          Node* prnt);

    
    
    
    
    static LHSprite* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHSprite();
    virtual ~LHSprite();
    
    bool initWithFilename(const std::string& filename, const std::string& folder, Node* prnt);
    bool initWithSpriteName(const std::string& spriteName, const std::string& imageFile, const std::string& folder, Node* prnt);
    

    
    static  bool isLHSprite(Node* obj){return (0 != dynamic_cast<LHSprite*>(obj));}
    virtual bool isSprite(){return true;}
        

#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif

    
#if LH_USE_BOX2D

    virtual void setPosition(const cocos2d::Vec2 &pos);
    virtual void setRotation(float rotation);
    
    virtual void setScaleX(float scaleX);
    virtual void setScaleY(float scaleY);
    virtual void setScale(float scaleX, float scaleY);
    
    virtual void updatePosition(const cocos2d::Vec2 &pos);
    virtual void updateRotation(float rotation);
#endif
    
    std::string getImageFilePath(){return imageFilePath;}
    std::string getSpriteFrameName(){return spriteFrameName;}
    
private:

    std::string imageFilePath;
    std::string spriteFrameName;
    
    void cacheSpriteFramesInfo(const std::string& imageFilePath, LHScene* scene);
};

#endif //__LEVELHELPER_API_SPRITE_H__//