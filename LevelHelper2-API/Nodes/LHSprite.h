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

using namespace cocos2d;

/**
 LHSprite class is used to load textured rectangles that are found in a level file.
 Users can retrieve a sprite object by calling the scene (LHScene) childNodeWithName: method.
 */

class LHDictionary;
class LHScene;

class LHSprite : public Sprite, public LHNodeProtocol, public LHNodeAnimationProtocol
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
     * @param   plistFile A null terminated string which indicates the plist containing the info of the sprite to be loaded.
     * @param   plistFolder A null terminated string which indicates the folder that contains the plist file. The folder must be added as reference in Xcode - blue icon.
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
     
     LHSprite* spr = LHSprite::createWithSpriteName("object_backpack", "objects.plist", "DEMO_PUBLISH_FOLDER/", this->getGameWorld());
     if(spr){
        spr->setPosition(Point(400,350));
     }
     @endcode
     */
    static LHSprite* createWithSpriteName(const std::string& spriteName,
                                          const std::string& plistFile,
                                          const std::string& plistFolder,
                                          Node* prnt);

    
    
    
    
    static LHSprite* spriteNodeWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHSprite();
    virtual ~LHSprite();
    
    bool initWithFilename(const std::string& filename, const std::string& folder, Node* prnt);
    bool initWithSpriteName(const std::string& spriteName, const std::string& plistFile, const std::string& plistFolder, Node* prnt);
    bool initWithDictionary(LHDictionary* dict, Node* prnt);

    
    static  bool isLHSprite(Node* obj){return (0 != dynamic_cast<LHSprite*>(obj));}
    virtual bool isSprite(){return true;}
        
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
    
private:

};

#endif //__LEVELHELPER_API_SPRITE_H__//