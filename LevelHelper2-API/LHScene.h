//
//  LHScene.h
//  LevelHelper2-Cocos2d-v3
//
//  Created by Bogdan Vladu on 24/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_SCENE_H__
#define __LEVELHELPER_API_SCENE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"

/**
 LHScene class is used to load a level file into Cocos2d-X v3 engine.
 End users will have to subclass this class in order to add they're own game logic.
 */
using namespace cocos2d;

class LHDevice;
class LHNode;
class LHDictionary;

class LHScene : public cocos2d::Scene, public LHNodeProtocol
{
    
public:
    static LHScene* createWithContentOfFile(const std::string& plistLevelFile);

    LHScene();
    virtual ~LHScene();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);
    
    std::string getCurrentDeviceSuffix();
    Size        getDesignResolutionSize();
    Point       getDesignOffset();

    LHNode* getGameWorld();
    LHNode* getUI();
    
    
    /**
     Returns the game world rectangle or CGRectZero if the game world rectangle is not set in the level file.
     */
    Rect        getGameWorldRect();
    
    /**
     Returns the informations that can be used to create an asset dynamically by specifying the file name.
     The asset file must be in the same folder as the scene file.
     If the asset file is not found it will return nil.
     
     @param assetFileName The name of the asset that. Do not provide an extension. E.g If file is named "myAsset.lhasset.plist" then yous should pass @"myAsset.lhasset"
     @return A dictionary containing the asset information or nullptr.
     */
    __Dictionary* assetInfoForFile(const std::string& assetFileName);

    
    static Point positionForNode(Node* node, Point unitPos);
    
    static  bool isLHScene(Node* obj){return (0 != dynamic_cast<LHScene*>(obj));}
    virtual bool isScene(){return true;};
    
    __Array* tracedFixturesWithUUID(const std::string& uuid);
    
private:
    
    friend class LHSprite;
    friend class LHNode;
    friend class LHNodeProtocol;
    friend class LHBezier;
    friend class LHCamera;
    friend class LHShape;
    friend class LHAsset;
    friend class LHParallaxLayer;
    friend class LHParallax;
    
    LHNode* _gameWorld;
    LHNode* _ui;
    
    std::vector<LHDevice*> devices;
    Size    designResolutionSize;
    Point   designOffset;
    Rect    gameWorldRect;
    __Dictionary* tracedFixtures;
    __Dictionary* _loadedAssetsInformations;
    
    static Node* createLHNodeWithDictionary(LHDictionary* childInfo, Node* prnt);
    
    void createPhysicsBoundarySectionFrom(Point from, Point to, const std::string& sectionName);
    
};

#endif //__LEVELHELPER_API_SCENE_H__
