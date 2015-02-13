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
#include "LHContactInfo.h"
#include "Box2D/Box2D.h"
#endif

/**
 LHScene class is used to load a level file into Cocos2d-X v3 engine.
 End users will have to subclass this class in order to add they're own game logic.
 */

class LHDevice;
class LHBackUINode;
class LHGameWorldNode;
class LHUINode;
class LHDictionary;
class LHAnimation;
class LHRopeJointNode;

#if LH_USE_BOX2D
class LHBox2dCollisionHandling;
#endif

typedef Node* (*LevelHelperNodeTypeSubclass)(LHDictionary* dict, Node* prnt);

/**
 LHScene class is used to load a level file into Cocos2d-X v3 engine.
 End users will have to subclass this class in order to add they're own game logic.
 */
class LHScene : public cocos2d::Scene, public LHNodeProtocol
{
    
public:
    
    /**
     Returns a scene object with all the level elements loaded into Cocos2d-X.
     */
    static LHScene* createWithContentOfFile(const std::string& plistLevelFile);

    LHScene();
    virtual ~LHScene();
    
    /**
     Overwrite this method to create your own auto-released LHScene subclass.
     
     :param: std::string The level file name. e.g "LH-Published/level.plist"
     
     :returns: true or false if level file was loaded.
     */
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
    
    
    CC_DEPRECATED_ATTRIBUTE virtual bool onTouchBegan(Touch* touch, Event* event); //use onTouchesBegan instead
    CC_DEPRECATED_ATTRIBUTE virtual void onTouchMoved(Touch* touch, Event* event);//use onTouchesMoved instead
    CC_DEPRECATED_ATTRIBUTE virtual void onTouchEnded(Touch* touch, Event* event);//use onTouchesEnded instead
    CC_DEPRECATED_ATTRIBUTE virtual void onTouchCancelled(Touch *touch, Event *event);//use onTouchesCancelled instead
    
    /**
     *  Overwrite this method to receive touch began events.
     *  Don't forget to call LHScene::onTouchesBegan(...) or else you will capture the events.
     *
     *  @param touches A vector of Touch objects.
     *  @param event   The event.
     */
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);

    /**
     *  Overwrite this method to receive touch moved events.
     *  Don't forget to call LHScene::onTouchesMoved(...) or else you will capture the events.
     *
     *  @param touches A vector of Touch objects.
     *  @param event   The event.
     */
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    
    /**
     *  Overwrite this method to receive touch ended events.
     *  Don't forget to call LHScene::onTouchesEnded(...) or else you will capture the events.
     *
     *  @param touches A vector of Touch objects.
     *  @param event   The event.
     */
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
    
    /**
     *  Overwrite this method to receive touch cancelled events.
     *  Don't forget to call LHScene::onTouchesCancelled(...) or else you will capture the events.
     *
     *  @param touches A vector of Touch objects.
     *  @param event   The event.
     */
    virtual void onTouchesCancelled(const std::vector<Touch*>& touches, Event *event);

    /**
     *  Overwrite this method to receive mouse scroll events.
     *  Don't forget to call LHScene::onMouseScroll(...) or else you will capture the event.
     *
     *  @param event   The event.
     */
    virtual void onMouseScroll(Event* event);
    
    /**
     *  Overwrite this method to receive pinch events.
     *  Don't forget to call LHScene::didPinchAtLocation(...) or else you will capture the event.
     *
     *  @param centerTouch The center point of the pinch.
     *  @param delta       How much the fingers have distance themself.
     *  @param closing     If the pinch is closeing or opening.
     */
    virtual void didPinchAtLocation(Point centerTouch, float delta, bool closing);

    /**
     *  Overwrite this method to receive onEnter events.
     *  Mandatory call LHScene::onEnter(); or else you will capture the event.
     */
    virtual void onEnter();
    
    /**
     *  Overwrite this method to receive onExit events.
     *  Mandatory call LHScene::onExit(); or else you will capture the event.
     */
    virtual void onExit();
    
    
#pragma mark- NODES SUBCLASSING
    
    /**
     Overwrite this method to return your own class type for specific nodes.
     Setup the class type in "Subclass" property of LevelHelper 2.
     Check LHSceneNodesSubclassingDemo for how to use this method.

     Your need to implement this function
        static YourClassType* nodeWithDictionary(LHDictionary* dict, Node* prnt);
     and overwrite this method
        virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
     
     @param subclassTypeName The name of the your custom class.
     @param lhTypeName The name of the original LevelHelper node class type. Your class must be a subclass of this type.
     */
    virtual LevelHelperNodeTypeSubclass createNodeObjectForSubclassWithName(const std::string subclassTypeName,
                                                                            const std::string lhTypeName)
    {return nullptr;}

    
#if LH_USE_BOX2D
    b2World* getBox2dWorld();
    float getPtm();
    
    b2Vec2 metersFromPoint(Point point);
    Point pointFromMeters(b2Vec2 vec);
    
    float metersFromValue(float val);
    float valueFromMeters(float meter);
    
    void setBox2dFixedTimeStep(float val); //default 1.0f / 120.0f;
    void setBox2dMinimumTimeStep(float val); //default 1.0f/600f;
    void setBox2dVelocityIterations(int val);//default 8
    void setBox2dPositionIterations(int val);//default 8
    void setBox2dMaxSteps(int val); //default 1

    
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
    virtual void didFinishedPlayingAnimation(LHAnimation* anim){}
    /**
     Overwrite this method to receive notifications when an animation has finished playing a repetition.
     */
    virtual void didFinishedRepetitionOnAnimation(LHAnimation* anim){}

#pragma mark- ROPE CUTTING
    
    /**
     Overwrite this method to receive notifications when a rope joint is cut.
     */
    virtual void didCutRopeJoint(LHRopeJointNode* joint){}

    
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
     
     @param contact The info of this contact. An object with nodeA, nodeB, nodeAShapeName, nodeBShapeName, nodeAShapeID, nodeBShapeID.
     
     */
    virtual void didBeginContact(const LHContactInfo& contact){}
    CC_DEPRECATED_ATTRIBUTE virtual void didBeginContactBetweenNodes(Node* nodeA, Node* nodeB, Point contactPoint, float impulse){}
    /**
     Overwrite this methods to receive collision informations when using Box2d.
     Called when the collision ends. Called when two nodes no longer collide at a specific point. May be called multiple times for same two nodes, because the point at which the nodes are touching has changed.
     
     Available only when using Box2d.
     
     @param LHContactInfo The info of this contact. NOTE: Contact Point and Impulse is not available at this stage.
     */
    virtual void didEndContact(const LHContactInfo& contact){}
    CC_DEPRECATED_ATTRIBUTE virtual void didEndContactBetweenNodes(Node* nodeA, Node* nodeB){}
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
    friend class LHPhysicsProtocol;
    
    LHBackUINode* _backUINode;
    LHGameWorldNode* _gameWorldNode;
    LHUINode* _uiNode;
    
    std::vector<LHDevice*> devices;
    LHDevice*              _currentDev;
    Size    designResolutionSize;
    Point   designOffset;
    Rect    gameWorldRect;
    
    EventListenerTouchAllAtOnce* _touchListener;
    
    EventListenerMouse* _mouseListener;
    
    __Dictionary*   _tracedFixtures;
    std::map<std::string, std::map<std::string, Value*> >_editorBodiesInfo; //<imageFile < spriteFrameName, bodyInfo> >
    __Dictionary*   _loadedAssetsInformations;
    __Array*        _lateLoadingNodes;
    
    void createPhysicsBoundarySectionFrom(Point from, Point to, const std::string& sectionName);
    
    void addLateLoadingNode(Node* nd);
    void performLateLoading();
    
    Point _ropeJointsCutStartPt;
    
    void loadGlobalGravityFromDictionary(LHDictionary* dict);
    void loadPhysicsBoundariesFromDictionary(LHDictionary* dict);
    void loadGameWorldInfoFromDictionary(LHDictionary* dict);
    
    bool hasEditorBodyInfoForImageFilePath(const std::string& imageFilePath);
    Value* getEditorBodyInfoForSpriteName(const std::string& spriteName, const std::string& imageFilePath);
    void setEditorBodyInfoForSpriteName(const std::string& spriteName, const std::string& imageFilePath, Value& bodyInfo);
    
    
    bool loadingInProgress;
    
#if LH_USE_BOX2D
    LHBox2dCollisionHandling* _box2dCollision;
    __Array* _physicsBoundarySubShapes;
#endif
    
    //pinch support
    float lastDistanceBetweenTouches;
};

#endif //__LEVELHELPER_API_SCENE_H__
