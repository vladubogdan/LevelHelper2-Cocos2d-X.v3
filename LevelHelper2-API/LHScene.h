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
class LHAnimation;

#if LH_USE_BOX2D
class LHBox2dCollisionHandling;
#endif

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

#pragma mark - ANIMATIONS HANDLING
    
    /**
     Overwrite this method to receive notifications when an animation has finished playing a repetition.
     */
    virtual void didFinishedPlayingAnimation(LHAnimation* anim){};
    /**
     Overwrite this method to receive notifications when an animation has finished playing a repetition.
     */
    virtual void didFinishedRepetitionOnAnimation(LHAnimation* anim){};

    
#pragma mark - COLLISION HANDLING

#if LH_USE_BOX2D
    /**
     Overwrite this methods to receive collision informations when using Box2d.
     This method is called prior the collision happening and lets the user decide whether or not the collision should happen.
     
     Available only when using Box2d.
     
     Useful when you have a character that jumps from platform to platform. When the character is under the platform you want to disable collision, but once the character is on top of the platform you want the collision to be triggers in order for the character to stay on top of the platform.
     
     @param nodeA First node that participates in the collision.
     @param nodeB Second node that participates in the collision.
     @return A boolean value telling whether or not the 2 nodes should collide.
     */
    virtual bool shouldDisableContactBetweenNodes(Node* nodeA, Node* nodeB){return false;}
    /**
     Overwrite this methods to receive collision informations when using Box2d.
     Called when the collision begins. Called with every new contact point between two nodes. May be called multiple times for same two nodes, because the point at which the nodes are touching has changed.
     
     Available only when using Box2d.
     
     @param nodeA First node that participates in the collision.
     @param nodeB Second node that participates in the collision.
     @param contactPoint The location where the two nodes collided in scene coordinates.
     @param impulse The impulse of the collision.
     
     */
    virtual void didBeginContactBetweenNodes(Node* nodeA, Node* nodeB, Point contactPoint, float impulse){};
    /**
     Overwrite this methods to receive collision informations when using Box2d.
     Called when the collision ends. Called when two nodes no longer collide at a specific point. May be called multiple times for same two nodes, because the point at which the nodes are touching has changed.
     
     Available only when using Box2d.
     
     @param nodeA First node that participates in the collision.
     @param nodeB Second node that participates in the collision.
     */
    virtual void didEndContactBetweenNodes(Node* nodeA, Node* nodeB){};
#else
    
    bool onContactBegin(PhysicsContact& contact);
    
#endif

    
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
    
#if LH_USE_BOX2D
    LHBox2dCollisionHandling* _box2dCollision;
#endif

};

#endif //__LEVELHELPER_API_SCENE_H__
