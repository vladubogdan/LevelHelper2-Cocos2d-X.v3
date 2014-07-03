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
#include "LHConfig.h"
#include "LHNodeProtocol.h"


#if LH_USE_BOX2D
#include "Box2D/Box2D.h"
#endif

/**
 LHScene class is used to load a level file into Cocos2d-X v3 engine.
 End users will have to subclass this class in order to add they're own game logic.
 */
using namespace cocos2d;

class LHDevice;
class LHBackUINode;
class LHGameWorldNode;
class LHUINode;
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

    LHBackUINode* getBackUINode();
    LHGameWorldNode* getGameWorldNode();
    LHUINode* getUINode();

    
    
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

    
    static  bool isLHScene(Node* obj){return (0 != dynamic_cast<LHScene*>(obj));}
    virtual bool isScene(){return true;};
    
    __Array* tracedFixturesWithUUID(const std::string& uuid);
    
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch *touch, Event *event);

    virtual void onEnter();
    virtual void onExit();
    
    
#if LH_USE_BOX2D
    b2World* getBox2dWorld();
    float getPtm();
    
    b2Vec2 metersFromPoint(Point point);
    Point pointFromMeters(b2Vec2 vec);
    
    float metersFromValue(float val);
    float valueFromMeters(float meter);
    
#endif //LH_USE_BOX2D
    
    
    /*Get the global gravity force.
     */
    Point getGlobalGravity();
    /*Sets the global gravity force
     @param gravity A point representing the gravity force in x and y direction.
     */
    void setGlobalGravity(Point gravity);
    
private:
    
    friend class LHBackUINode;
    friend class LHUINode;
    friend class LHGameWorldNode;
    friend class LHSprite;
    friend class LHNode;
    friend class LHNodeProtocol;
    friend class LHBezier;
    friend class LHCamera;
    friend class LHShape;
    friend class LHAsset;
    friend class LHParallaxLayer;
    friend class LHParallax;
    friend class LHRopeJointNode;

    LHBackUINode* _backUINode;
    LHGameWorldNode* _gameWorldNode;
    LHUINode* _uiNode;
    
    std::vector<LHDevice*> devices;
    Size    designResolutionSize;
    Point   designOffset;
    Rect    gameWorldRect;
    
    EventListenerTouchOneByOne* _touchListener;
    
    __Dictionary*   _tracedFixtures;
    __Dictionary*   _loadedAssetsInformations;
    __Array*        _lateLoadingNodes;
    
    void createPhysicsBoundarySectionFrom(Point from, Point to, const std::string& sectionName);
    
    void addLateLoadingNode(Node* nd);
    void performLateLoading();
    
    Point _ropeJointsCutStartPt;
    
    void loadGlobalGravityFromDictionary(LHDictionary* dict);
    void loadPhysicsBoundariesFromDictionary(LHDictionary* dict);
};

#endif //__LEVELHELPER_API_SCENE_H__
