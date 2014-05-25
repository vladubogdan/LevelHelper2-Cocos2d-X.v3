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

class LHScene : public cocos2d::Scene, LHNodeProtocol
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
    
    Rect        getGameWorldRect();
    
    static Point positionForNode(Node* node, Point unitPos);
    
    static  bool isLHScene(Node* obj){return (0 != dynamic_cast<LHScene*>(obj));}
    virtual bool isScene(){return true;};
    
    __Array* tracedFixturesWithUUID(const std::string& uuid);
    
//    const kmMat4& getParentToNodeTransform() const;
//    const kmMat4& getNodeToParentTransform() const;
//    kmMat4& getNodeToParentTransformTest() const;
    
private:
    
    friend class LHSprite;
    friend class LHNode;
    friend class LHNodeProtocol;
    friend class LHBezier;
    friend class LHCamera;
    friend class LHShape;
    
    LHNode* _gameWorld;
    LHNode* _ui;
    
    std::vector<LHDevice*> devices;
    Size    designResolutionSize;
    Point   designOffset;
    Rect    gameWorldRect;
    __Dictionary* tracedFixtures;
    
    static Node* createLHNodeWithDictionary(LHDictionary* childInfo, Node* prnt);
    
    void createPhysicsBoundarySectionFrom(Point from, Point to, const std::string& sectionName);
    
    
//+(instancetype)sceneWithContentOfFile:(NSString*)levelPlistFile;
//-(instancetype)initWithContentOfFile:(NSString*)levelPlistFile;

/**
 Returns a CCTexture object that was previously loaded or a new one.
 @param imagePath The path of the image that needs to get returned as a texture.
 @return An initialized CCTexture Object.
 */
//-(CCTexture*)textureWithImagePath:(NSString*)imagePath;


/**
 Returns the game world rectangle or CGRectZero if the game world rectangle is not set in the level file.
 */
//-(CGRect)gameWorldRect;

/**
 Returns the informations that can be used to create an asset dynamically by specifying the file name. 
 The asset file must be in the same folder as the scene file.
 If the asset file is not found it will return nil.
 
 @param assetFileName The name of the asset that. Do not provide an extension. E.g If file is named "myAsset.lhasset.plist" then yous should pass @"myAsset.lhasset"
 @return A dictionary containing the asset information or nil.
 */
//-(NSDictionary*)assetInfoForFile:(NSString*)assetFileName;



#pragma mark - LHNodeProtocol

/**
 Returns the unique identifier of the node.
 */
//-(NSString*)uuid;

/**
 Returns all tag values of the node.
 */
//-(NSArray*)tags;

/**
 Returns the user property object assigned to this object or nil.
 */
//-(id<LHUserPropertyProtocol>)userProperty;

/**
 Returns the scene to which this node belongs to.
 */
//-(LHScene*)scene;

/**
 Returns a node with the specified unique name or nil if that node is not found in the children hierarchy.
 @param name The unique name of the node.
 @return A node or or nil.
 */
//-(CCNode <LHNodeProtocol>*)childNodeWithName:(NSString*)name;

/**
 Returns a node with the specified unique identifier or nil if that node is not found in the children hierarchy.
 @param uuid The unique idenfier of the node.
 @return A node or or nil.
 */
//-(CCNode <LHNodeProtocol>*)childNodeWithUUID:(NSString*)uuid;

/**
 Returns all children nodes that have the specified tag values.
 @param tagValues An array containing tag names. Array of NSString's.
 @param any Specify if all or just one tag value of the node needs to be in common with the passed ones.
 @return A node or or nil.
 */
//-(NSMutableArray*)childrenWithTags:(NSArray*)tagValues containsAny:(BOOL)any;

/**
 Returns all children nodes that are of specified class type.
 @param type A "Class" type.
 @return An array with all the found nodes of the specified class.
 */
//-(NSMutableArray*)childrenOfType:(Class)type;

};

#endif //__LEVELHELPER_API_SCENE_H__
