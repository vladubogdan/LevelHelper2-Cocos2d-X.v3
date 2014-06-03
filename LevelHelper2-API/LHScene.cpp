//
//  LHScene.m
//  LevelHelper2-SpriteKit
//
//  Created by Bogdan Vladu on 24/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHScene.h"
#include "LHDictionary.h"
#include "LHArray.h"
#include "LHDevice.h"

#include "LHSprite.h"
#include "LHNode.h"
#include "LHCamera.h"
#include "LHBezier.h"
#include "LHShape.h"
#include "LHAsset.h"
#include "LHParallax.h"
#include "LHParallaxLayer.h"
#include "LHRopeJointNode.h"

#include "LHUtils.h"


#include <sstream>
using namespace std;


using namespace cocos2d;

LHScene::LHScene()
{
    _loadedAssetsInformations = nullptr;
    _tracedFixtures = nullptr;
    _gameWorld = nullptr;
    _lateLoadingNodes = nullptr;
    _ui = nullptr;
    
//    printf("lhscene constructor\n");
}

LHScene::~LHScene()
{
    for (size_t i = 0; i < devices.size(); ++i)
    {
        LHDevice* dev = devices[i];
        delete dev;
        devices[i] = NULL;
    }
    devices.clear();
    
    CC_SAFE_RELEASE(_tracedFixtures);
    _gameWorld = nullptr;
    _ui = nullptr;

    CC_SAFE_DELETE(_loadedAssetsInformations);
    
//    printf("lhscene dealloc\n");
}

bool LHScene::initWithContentOfFile(const std::string& plistLevelFile)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(plistLevelFile);
    
    LHDictionary* dict = (LHDictionary*)__Dictionary::createWithContentsOfFile(fullPath.c_str());
    
    int aspect = dict->intForKey("aspect");
    
    designResolutionSize = dict->sizeForKey("designResolution");
    LHArray* devsInfo = dict->arrayForKey("devices");
    
    for(int i = 0; i < devsInfo->count(); ++i)
    {
        LHDictionary* devInf = devsInfo->dictAtIndex(i);
        devices.push_back(new LHDevice(devInf));
    }
    
    LHDevice* curDev = LHDevice::currentDeviceFromArray(devices);
    
    Size sceneSize = curDev->getSize();
    
    float ratio = curDev->getRatio();
    sceneSize.width = sceneSize.width/ratio;
    sceneSize.height = sceneSize.height/ratio;
    
    Director::getInstance()->setContentScaleFactor(ratio);
    
    if(aspect == 0)//exact fit
    {
        sceneSize = designResolutionSize;
        
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize(designResolutionSize.width,
                                                                          designResolutionSize.height,
                                                                          ResolutionPolicy::EXACT_FIT);
    }
    else if(aspect == 1)//no borders
    {
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize(designResolutionSize.width,
                                                                          designResolutionSize.height,
                                                                          ResolutionPolicy::NO_BORDER);
    }
    else if(aspect == 2)//show all
    {
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize(sceneSize.width,
                                                                          sceneSize.height,
                                                                          ResolutionPolicy::SHOW_ALL);
        
        designOffset.x = (sceneSize.width - designResolutionSize.width)*0.5;
        designOffset.y = (sceneSize.height - designResolutionSize.height)*0.5;
    }
    
    bool ret = Scene::initWithPhysics();
    if(ret)
    {
        setContentSize(sceneSize);
        
        
        loadGenericInfoFromDictionary(dict);
        this->setName(plistLevelFile);

        
//            [self setName:levelPlistFile];
//            _uuid = [[NSString alloc] initWithString:[dict objectForKey:@"uuid"]];
//            _userProperty = [LHUtils userPropertyForNode:self fromDictionary:dict];
//            [LHUtils tagsFromDictionary:dict
//                           savedToArray:&_tags];
        
            LHDictionary* tracedFixInfo = dict->dictForKey("tracedFixtures");
            if(tracedFixInfo){
                _tracedFixtures = __Dictionary::createWithDictionary(tracedFixInfo);
                _tracedFixtures->retain();
            }

//        this->setAnchorPoint(Point(0,1));
        
        _gameWorld = LHNode::createWithName("LH_GAME_WORLD_NODE");
        _gameWorld->setLocalZOrder(0);
//        _gameWorld->setAnchorPoint(Point(0,0));
        
        _gameWorld->setContentSize(sceneSize);
        this->addChild(_gameWorld);
        
        _ui = LHNode::createWithName("LH_UI_NODE");
        _ui->setLocalZOrder(1);
        this->addChild(_ui);

        
        //load background color
        Color3B backgroundClr = dict->colorForKey("backgroundColor");
        glClearColor(backgroundClr.r, backgroundClr.g, backgroundClr.b, 1.0f);

        
        getPhysicsWorld()->setDebugDrawMask(true ? PhysicsWorld::DEBUGDRAW_ALL : PhysicsWorld::DEBUGDRAW_NONE);

        
        if(dict->boolForKey("useGlobalGravity"))
        {
            Point gravityVector = dict->pointForKey("globalGravityDirection");
            float gravityForce  = dict->floatForKey("globalGravityForce");
            
            this->getPhysicsWorld()->setGravity(Point(gravityVector.x*gravityForce*100,
                                                      gravityVector.y*gravityForce*100));
            
        }
        
        
        LHDictionary* phyBoundInfo = dict->dictForKey("physicsBoundaries");
        if(phyBoundInfo)
        {
            Size scr = LH_SCREEN_RESOLUTION;
            
            stringstream tempString;
            tempString <<(int)scr.width<<"x"<<(int)scr.height;
            std::string key = tempString.str(); //Converts this into string;

            std::string rectInf;
            if(phyBoundInfo->objectForKey(key)){
                rectInf = phyBoundInfo->stringForKey(key);

            }
            else{
                rectInf = phyBoundInfo->stringForKey("general");
            }
            
            if(rectInf.length() > 0){
                Rect bRect = RectFromString(rectInf);
                Size designSize = getDesignResolutionSize();
                Point offset = getDesignOffset();
                Rect skBRect(bRect.origin.x*designSize.width + offset.x,
                             getContentSize().height -  bRect.origin.y*designSize.height + offset.y,
                             bRect.size.width*designSize.width ,
                             -bRect.size.height*designSize.height);
                                
                {
                    createPhysicsBoundarySectionFrom(Point(skBRect.getMinX(), skBRect.getMinY()),
                                                     Point(skBRect.getMaxX(), skBRect.getMinY()),
                                                     "LHPhysicsBottomBoundary");
                }
                
                {
                    createPhysicsBoundarySectionFrom(Point(skBRect.getMaxX(), skBRect.getMinY()),
                                                     Point(skBRect.getMaxX(), skBRect.getMaxY()),
                                                    "LHPhysicsRightBoundary");
                    
                }
                
                {
                    createPhysicsBoundarySectionFrom(Point(skBRect.getMaxX(), skBRect.getMaxY()),
                                                     Point(skBRect.getMinX(), skBRect.getMaxY()),
                                                     "LHPhysicsTopBoundary");
                }
                
                {
                    createPhysicsBoundarySectionFrom(Point(skBRect.getMinX(), skBRect.getMaxY()),
                                                     Point(skBRect.getMinX(), skBRect.getMinY()),
                                                    "LHPhysicsLeftBoundary");
                }
            }
        }
        
        
        
        
        LHDictionary* gameWorldInfo = dict->dictForKey("gameWorld");
        if(gameWorldInfo)
        {
            Size scr = LH_SCREEN_RESOLUTION;
            
            stringstream tempString;
            tempString <<(int)scr.width<<"x"<<(int)scr.height;
            std::string key = tempString.str(); //Converts this into string;
            
            std::string rectInf;
            if(gameWorldInfo->objectForKey(key)){
                rectInf = gameWorldInfo->stringForKey(key);
                
            }
            else{
                rectInf = gameWorldInfo->stringForKey("general");
            }
            
            if(rectInf.length() > 0){
                Rect bRect = RectFromString(rectInf);
                Size designSize = getDesignResolutionSize();
                Point offset = getDesignOffset();
                
                gameWorldRect = Rect(bRect.origin.x*designSize.width+ offset.x,
                                     (1.0f-  bRect.origin.y)*designSize.height + offset.y,
                                     bRect.size.width*designSize.width ,
                                     -(bRect.size.height)*designSize.height);
                                
//                DrawNode* gw = DrawNode::create();
//                
//                gw->drawSegment(Point(gameWorldRect.getMinX(), gameWorldRect.getMinY()),
//                                Point(gameWorldRect.getMaxX(), gameWorldRect.getMinY()), 1, Color4F::BLUE);
//                
//                gw->drawSegment(Point(gameWorldRect.getMaxX(), gameWorldRect.getMinY()),
//                                                 Point(gameWorldRect.getMaxX(), gameWorldRect.getMaxY()), 1, Color4F::BLUE);
//                
//                gw->drawSegment(Point(gameWorldRect.getMaxX(), gameWorldRect.getMaxY()),
//                                                 Point(gameWorldRect.getMinX(), gameWorldRect.getMaxY()), 1, Color4F::BLUE);
//                gw->drawSegment(Point(gameWorldRect.getMinX(), gameWorldRect.getMaxY()),
//                                                 Point(gameWorldRect.getMinX(), gameWorldRect.getMinY()), 1, Color4F::BLUE);
//
//                _gameWorld->addChild(gw);
            }
        }
        
        
        
        
        LHArray* childrenInfo = dict->arrayForKey("children");
        for(int i = 0; i < childrenInfo->count(); ++i)
        {
            LHDictionary* childInfo = childrenInfo->dictAtIndex(i);
            
            Node* node = LHScene::createLHNodeWithDictionary(childInfo, _gameWorld);
            
            if(node){
                
            }
        }
        
        
        this->performLateLoading();
        
        
        ret = true;
    };
    
    return ret;
}


LHScene *LHScene::createWithContentOfFile(const std::string& plistLevelFile)
{
//    printf("lhscene createWithcontent\n");
    
    LHScene *ret = new LHScene();
    if (ret && ret->initWithContentOfFile(plistLevelFile))
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

void LHScene::addLateLoadingNode(Node* node){
    if(!_lateLoadingNodes) {
        _lateLoadingNodes = new __Array();
        _lateLoadingNodes->init();
    }
    _lateLoadingNodes->addObject(node);
}

void LHScene::performLateLoading(){
    if(!_lateLoadingNodes)return;
    
    for(size_t i = 0; i < _lateLoadingNodes->count(); ++i)
    {
        Node* node = (Node*)_lateLoadingNodes->getObjectAtIndex(i);

        LHNodeProtocol* protocol = dynamic_cast<LHNodeProtocol*>(node);
        if(protocol)
        {
            protocol->lateLoading();
        }
    }
    
    CC_SAFE_DELETE(_lateLoadingNodes);
}


Node* LHScene::createLHNodeWithDictionary(LHDictionary* childInfo, Node* prnt)
{
    
    std::string nodeType = childInfo->stringForKey("nodeType");
    
    LHScene* scene = NULL;
    if( LHScene::isLHScene(prnt)){
        scene = (LHScene*)prnt;
    }
    else if(LHScene::isLHScene(prnt->getScene())){
        scene = (LHScene*)prnt->getScene();
    }
    
    
    if(nodeType == "LHSprite")
    {
        LHSprite* spr = LHSprite::spriteNodeWithDictionary(childInfo, prnt);
        return spr;
    }
    else if(nodeType == "LHNode")
    {
        LHNode* nd = LHNode::nodeWithDictionary(childInfo, prnt);
        return nd;
    }
    else if(nodeType == "LHCamera")
    {
        LHCamera* cm = LHCamera::cameraWithDictionary(childInfo, prnt);
        return cm;
    }
    else if(nodeType == "LHBezier")
    {
        LHBezier* bez = LHBezier::bezierNodeWithDictionary(childInfo, prnt);
        return bez;
    }
    else if(nodeType == "LHTexturedShape")
    {
        LHShape* sp = LHShape::shapeNodeWithDictionary(childInfo, prnt);
        return sp;
    }
    else if(nodeType == "LHAsset")
    {
        LHAsset* as = LHAsset::assetNodeWithDictionary(childInfo, prnt);
        return as;
    }
    else if(nodeType == "LHParallax")
    {
        LHParallax* pr = LHParallax::parallaxWithDictionary(childInfo, prnt);
        return pr;
    }
    else if(nodeType == "LHParallaxLayer")
    {
        LHParallaxLayer* lh = LHParallaxLayer::parallaxLayerWithDictionary(childInfo, prnt);
        return lh;
    }
    else if(nodeType == "LHRopeJoint")
    {
        if(scene)
        {
            LHRopeJointNode* jt = LHRopeJointNode::ropeJointNodeWithDictionary(childInfo, prnt);
            scene->addLateLoadingNode(jt);
        }
    }

//    else if([nodeType isEqualToString:@"LHWaves"])
//    {
//        LHWater* wt = [LHWater waterNodeWithDictionary:childInfo
//                                                parent:prnt];
//        return wt;
//    }
//    else if([nodeType isEqualToString:@"LHAreaGravity"])
//    {
//        LHGravityArea* gv = [LHGravityArea gravityAreaWithDictionary:childInfo
//                                                              parent:prnt];
//        return gv;
//    }
    //    else if([nodeType isEqualToString:@"LHWeldJoint"])
    //    {
    //        LHWeldJointNode* jt = [LHWeldJointNode weldJointNodeWithDictionary:childInfo
    //                                                                    parent:prnt];
    //        [scene addDebugJointNode:jt];
    //        [scene addLateLoadingNode:jt];
    //    }
    //    else if([nodeType isEqualToString:@"LHRevoluteJoint"]){
    //
    //        LHRevoluteJointNode* jt = [LHRevoluteJointNode revoluteJointNodeWithDictionary:childInfo
    //                                                                                parent:prnt];
    //
    //        [scene addDebugJointNode:jt];
    //        [scene addLateLoadingNode:jt];
    //    }
//    else if([nodeType isEqualToString:@"LHDistanceJoint"]){
//        
//        LHDistanceJointNode* jt = [LHDistanceJointNode distanceJointNodeWithDictionary:childInfo
//                                                                                parent:prnt];
//        [scene addLateLoadingNode:jt];
//        
//    }
    //    else if([nodeType isEqualToString:@"LHPrismaticJoint"]){
    //
    //        LHPrismaticJointNode* jt = [LHPrismaticJointNode prismaticJointNodeWithDictionary:childInfo
    //                                                                                   parent:prnt];
    //        [scene addDebugJointNode:jt];
    //        [scene addLateLoadingNode:jt];
    //    }
    
    
    else{
        printf("UNKNOWN NODE TYPE %s\n", nodeType.c_str());
    }
    
    return NULL;
}

void LHScene::createPhysicsBoundarySectionFrom(Point from, Point to, const std::string& sectionName)
{
    LHNode* drawNode = LHNode::createWithName(sectionName);
    PhysicsBody* boundaryBody = PhysicsBody::createEdgeSegment(from, to);
    drawNode->setPhysicsBody(boundaryBody);
    _gameWorld->addChild(drawNode);
}

__Array* LHScene::tracedFixturesWithUUID(const std::string& uuid)
{
    return (__Array*)_tracedFixtures->objectForKey(uuid);
}

std::string LHScene::getCurrentDeviceSuffix()
{
    LHDevice* dev = LHDevice::currentDeviceFromArray(devices);
    
    if(dev){
        return dev->getSuffix();
    }
    return "";
}
Size LHScene::getDesignResolutionSize(){
    return designResolutionSize;
}

Point LHScene::getDesignOffset(){
    return designOffset;
}

LHNode* LHScene::getGameWorld(){
    return _gameWorld;
}
LHNode* LHScene::getUI(){
    return _ui;
}

Rect LHScene::getGameWorldRect(){
    return gameWorldRect;
}

__Dictionary* LHScene::assetInfoForFile(const std::string& assetFileName)
{
    if(!_loadedAssetsInformations){
        _loadedAssetsInformations = new __Dictionary();
        _loadedAssetsInformations->init();
    }
    
    __Dictionary* info = (__Dictionary*)_loadedAssetsInformations->objectForKey(assetFileName);
    if(!info){
        
        std::string folder = LHUtils::removeLastPathComponent(this->getName());
        
        std::string path = FileUtils::getInstance()->fullPathForFilename(folder + assetFileName + ".plist");
        
        if(path.length()>0){
            info =  __Dictionary::createWithContentsOfFile(path.c_str());
            if(info){
                _loadedAssetsInformations->setObject(info, assetFileName);
            }
        }
    }
    return info;
}


Point LHScene::positionForNode(Node* node, Point unitPos)
{
    LHScene* scene = (LHScene*)node->getScene();

    Size designSize = scene->getDesignResolutionSize();
    Point offset    = scene->getDesignOffset();
    
    Point designPos = Point();
    
    if(node->getParent() == scene->getGameWorld() || node->getParent() == scene){// scene->physicsNode()){
        
        designPos = Point(designSize.width*unitPos.x,
                          (designSize.height - designSize.height*unitPos.y));
        designPos.x += offset.x;
        designPos.y += offset.y;

    }
    else{
        designPos = Point(designSize.width*unitPos.x,
                          (node->getParent()->getContentSize().height - designSize.height*unitPos.y));
        
        Node* p = node->getParent();
        designPos.x += p->getContentSize().width*0.5;
        designPos.y -= p->getContentSize().height*0.5;
    }
    
    return designPos;
}



//#import "LHUtils.h"
//#import "NSDictionary+LHDictionary.h"
//#import "LHSprite.h"
//#import "LHBezier.h"
//#import "LHShape.h"
//#import "LHWater.h"
//#import "LHNode.h"
//#import "LHAsset.h"
//#import "LHGravityArea.h"
//#import "LHParallax.h"
//#import "LHParallaxLayer.h"
//#import "LHCamera.h"
//#import "LHRopeJointNode.h"
//#import "LHWeldJointNode.h"
//#import "LHRevoluteJointNode.h"
//#import "LHDistanceJointNode.h"
//#import "LHPrismaticJointNode.h"

/*
@implementation LHScene
{
    CCPhysicsNode *__unsafe_unretained physicsNode;
    
    NSMutableArray* lateLoadingNodes;//gets nullified after everything is loaded
    
    NSString* _uuid;
    NSArray* _tags;
    id<LHUserPropertyProtocol> _userProperty;
    
    NSMutableDictionary* loadedTextures;
    NSMutableDictionary* loadedTextureAtlases;
    NSDictionary* tracedFixtures;
    
    NSArray* supportedDevices;

    CGSize  designResolutionSize;
    CGPoint designOffset;
    
    NSString* relativePath;
    
    CCNode* touchedNode;
    BOOL touchedNodeWasDynamic;
    
    NSMutableArray* debugJoints;//its only used when in debug mode;
    CGPoint ropeJointsCutStartPt;
    
    NSMutableArray* gravityNodes;
    
    NSMutableDictionary* _loadedAssetsInformations;
    
    CGRect gameWorldRect;
    NSMutableArray* cameras;
    
    NSTimeInterval previousUpdateTime;
}


-(void)dealloc{
    physicsNode = nil;
    
    LH_SAFE_RELEASE(_uuid);
    LH_SAFE_RELEASE(_userProperty);
    LH_SAFE_RELEASE(_tags);
    
    LH_SAFE_RELEASE(relativePath);
    LH_SAFE_RELEASE(gravityNodes);
    LH_SAFE_RELEASE(loadedTextures);
    LH_SAFE_RELEASE(loadedTextureAtlases);
    LH_SAFE_RELEASE(tracedFixtures);
    LH_SAFE_RELEASE(supportedDevices);
    LH_SAFE_RELEASE(cameras);
    LH_SAFE_RELEASE(_loadedAssetsInformations);
    
    LH_SUPER_DEALLOC();
}
 

+(instancetype)sceneWithContentOfFile:(NSString*)levelPlistFile{
    return LH_AUTORELEASED([[self alloc] initWithContentOfFile:levelPlistFile]);
}

-(instancetype)initWithContentOfFile:(NSString*)levelPlistFile
{
    NSString* path = [[NSBundle mainBundle] pathForResource:[levelPlistFile stringByDeletingPathExtension]
                                                     ofType:[levelPlistFile pathExtension]];
    
    if(!path)return nil;
    
    NSDictionary* dict = [NSDictionary dictionaryWithContentsOfFile:path];
    if(!dict)return nil;

    int aspect = [dict intForKey:@"aspect"];
    CGSize designResolution = [dict sizeForKey:@"designResolution"];

    NSArray* devsInfo = [dict objectForKey:@"devices"];
    NSMutableArray* devices = [NSMutableArray array];
    for(NSDictionary* devInf in devsInfo){
        LHDevice* dev = [LHDevice deviceWithDictionary:devInf];
        [devices addObject:dev];
    }

    #if TARGET_OS_IPHONE
    LHDevice* curDev = [LHUtils currentDeviceFromArray:devices];
    #else
    LHDevice* curDev = [LHUtils deviceFromArray:devices withSize:size];
    #endif

    CGPoint childrenOffset = CGPointZero;
    
    CGSize sceneSize = curDev.size;
    float ratio = curDev.ratio;
    sceneSize.width = sceneSize.width/ratio;
    sceneSize.height = sceneSize.height/ratio;
    
    aspect = 2;//HARD CODED UNTIL I FIGURE OUT HOW TO DO IT IN v3
    
    if(aspect == 0)//exact fit
    {
        sceneSize = designResolution;
    }
    else if(aspect == 1)//no borders
    {
        float scalex = sceneSize.width/designResolution.width;
        float scaley = sceneSize.height/designResolution.height;
        scalex = scaley = MAX(scalex, scaley);
        
        childrenOffset.x = (sceneSize.width/scalex - designResolution.width)*0.5;
        childrenOffset.y = (sceneSize.height/scaley - designResolution.height)*0.5;
        sceneSize = CGSizeMake(sceneSize.width/scalex, sceneSize.height/scaley);
        
    }
    else if(aspect == 2)//show all
    {
        [[CCDirector sharedDirector] setDesignSize:designResolution];
        childrenOffset.x = (sceneSize.width - designResolution.width)*0.5;
        childrenOffset.y = (sceneSize.height - designResolution.height)*0.5;
    }

    if (self = [super init])
    {
        relativePath = [[NSString alloc] initWithString:[levelPlistFile stringByDeletingLastPathComponent]];
        
        designResolutionSize = designResolution;
        designOffset         = childrenOffset;
        [[CCDirector sharedDirector] setContentScaleFactor:ratio];
        
//        [self setAnchorPoint:CGPointMake(0.5, 0.5)];
        

        [self setName:levelPlistFile];
        _uuid = [[NSString alloc] initWithString:[dict objectForKey:@"uuid"]];
        _userProperty = [LHUtils userPropertyForNode:self fromDictionary:dict];
        [LHUtils tagsFromDictionary:dict
                       savedToArray:&_tags];
        
        NSDictionary* tracedFixInfo = [dict objectForKey:@"tracedFixtures"];
        if(tracedFixInfo){
            tracedFixtures = [[NSDictionary alloc] initWithDictionary:tracedFixInfo];
        }

        supportedDevices = [[NSArray alloc] initWithArray:devices];
        
        CCPhysicsNode* pNode = [CCPhysicsNode node];
        pNode.contentSize = self.contentSize;
        [pNode setDebugDraw:YES];
        [super addChild:pNode];
        physicsNode = pNode;
        
        
        
        if([dict boolForKey:@"useGlobalGravity"])
        {
            //more or less the same as box2d
            CGPoint gravityVector = [dict pointForKey:@"globalGravityDirection"];
            float gravityForce    = [dict floatForKey:@"globalGravityForce"];
            [physicsNode setGravity:CGPointMake(gravityVector.x*gravityForce,
                                                gravityVector.y*gravityForce*100)];
        }
        
        
        //load background color
        CCColor* backgroundClr = [dict colorForKey:@"backgroundColor"];
        glClearColor(backgroundClr.red, backgroundClr.green, backgroundClr.blue, 1.0f);

        
        NSArray* childrenInfo = [dict objectForKey:@"children"];
        for(NSDictionary* childInfo in childrenInfo)
        {
            CCNode* node = [LHScene createLHNodeWithDictionary:childInfo
                                                        parent:physicsNode];
            
            if(node){

            }
        }
        
        
        
        NSDictionary* phyBoundInfo = [dict objectForKey:@"physicsBoundaries"];
        if(phyBoundInfo)
        {
            CGSize scr = LH_SCREEN_RESOLUTION;

            NSString* rectInf = [phyBoundInfo objectForKey:[NSString stringWithFormat:@"%dx%d", (int)scr.width, (int)scr.height]];
            if(!rectInf){
                rectInf = [phyBoundInfo objectForKey:@"general"];
            }
            
            if(rectInf){
                CGRect bRect = LHRectFromString(rectInf);
                CGSize designSize = [self designResolutionSize];
                CGPoint offset = [self designOffset];
                CGRect skBRect = CGRectMake(bRect.origin.x*designSize.width + offset.x,
                                            self.contentSize.height - bRect.origin.y*designSize.height + offset.y,
                                            bRect.size.width*designSize.width ,
                                            -bRect.size.height*designSize.height);
                
                {
                    [self createPhysicsBoundarySectionFrom:CGPointMake(CGRectGetMinX(skBRect), CGRectGetMinY(skBRect))
                                                        to:CGPointMake(CGRectGetMaxX(skBRect), CGRectGetMinY(skBRect))
                                                  withName:@"LHPhysicsBottomBoundary"];
                }
                
                {
                    [self createPhysicsBoundarySectionFrom:CGPointMake(CGRectGetMaxX(skBRect), CGRectGetMinY(skBRect))
                                                        to:CGPointMake(CGRectGetMaxX(skBRect), CGRectGetMaxY(skBRect))
                                                  withName:@"LHPhysicsRightBoundary"];

                }
                
                {
                    [self createPhysicsBoundarySectionFrom:CGPointMake(CGRectGetMaxX(skBRect), CGRectGetMaxY(skBRect))
                                                        to:CGPointMake(CGRectGetMinX(skBRect), CGRectGetMaxY(skBRect))
                                                  withName:@"LHPhysicsTopBoundary"];
                }

                {
                    [self createPhysicsBoundarySectionFrom:CGPointMake(CGRectGetMinX(skBRect), CGRectGetMaxY(skBRect))
                                                        to:CGPointMake(CGRectGetMinX(skBRect), CGRectGetMinY(skBRect))
                                                  withName:@"LHPhysicsLeftBoundary"];
                }
            }
        }

        NSDictionary* gameWorldInfo = [dict objectForKey:@"gameWorld"];
        if(gameWorldInfo)
        {
#if TARGET_OS_IPHONE
            CGSize scr = LH_SCREEN_RESOLUTION;
#else
            CGSize scr = self.size;
#endif

            NSString* rectInf = [gameWorldInfo objectForKey:[NSString stringWithFormat:@"%dx%d", (int)scr.width, (int)scr.height]];
            if(!rectInf){
                rectInf = [gameWorldInfo objectForKey:@"general"];
            }
            
            if(rectInf){
                CGRect bRect = LHRectFromString(rectInf);
                CGSize designSize = [self designResolutionSize];
                CGPoint offset = [self designOffset];

                gameWorldRect = CGRectMake(bRect.origin.x*designSize.width+ offset.x,
                                           (1.0f - bRect.origin.y)*designSize.height + offset.y,
                                           bRect.size.width*designSize.width ,
                                           -(bRect.size.height)*designSize.height);
                gameWorldRect.origin.y -= sceneSize.height;
            }
        }
        
        [self performLateLoading];
        
        [self setUserInteractionEnabled:YES];

    }
    return self;
}

-(CCPhysicsNode*)physicsNode{
    return physicsNode;
}




-(void)addChild:(CCNode *)node{
    [physicsNode addChild:node];
}

//-(SKTextureAtlas*)textureAtlasWithImagePath:(NSString*)atlasPath
//{
//    if(!loadedTextureAtlases){
//        loadedTextureAtlases = [[NSMutableDictionary alloc] init];
//    }
// 
//    SKTextureAtlas* textureAtlas = nil;
//    if(atlasPath){
//        textureAtlas = [loadedTextureAtlases objectForKey:atlasPath];
//        if(!textureAtlas){
//            textureAtlas = [SKTextureAtlas atlasNamed:atlasPath];
//            if(textureAtlas){
//                [loadedTextureAtlases setObject:textureAtlas forKey:atlasPath];
//            }
//        }
//    }
//    
//    return textureAtlas;
//}



-(LHCamera*)cameraWithName:(NSString*)name
{
    for(LHCamera* cam in cameras){
        if([[cam name] isEqualToString:name]){
            return cam;
        }
    }
    return nil;
}
-(NSMutableArray*)cameras{
    return cameras;
}

-(CGRect)gameWorldRect{
    return gameWorldRect;
}

//#if TARGET_OS_IPHONE
//-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
//{
//    
//    CGVector grv = self.physicsWorld.gravity;
//    
//    [self.physicsWorld setGravity:CGVectorMake(grv.dx,
//                                              -grv.dy)];
//    
//    return;
//    
//    for (UITouch *touch in touches) {
//        CGPoint location = [touch locationInNode:self];
//        
//        ropeJointsCutStartPt = location;
//        
//        NSArray* foundNodes = [self nodesAtPoint:location];
//        for(SKNode* foundNode in foundNodes)
//        {
//            if(foundNode.physicsBody){
//                touchedNode = foundNode;
//                touchedNodeWasDynamic = touchedNode.physicsBody.affectedByGravity;
//                [touchedNode.physicsBody setAffectedByGravity:NO];                
//                return;
//            }
//        }
//    }
//}
//
//-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
//{
//    for (UITouch *touch in touches) {
//        CGPoint location = [touch locationInNode:self];
//
//        if(touchedNode && touchedNode.physicsBody){
//            [touchedNode setPosition:location];
//        }
//    }
//}
//- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
//    
//    for (UITouch *touch in touches) {
//        CGPoint location = [touch locationInNode:self];
//        
//        for(LHRopeJointNode* rope in ropeJoints){
//            if([rope canBeCut]){
//                [rope cutWithLineFromPointA:ropeJointsCutStartPt
//                                   toPointB:location];
//            }
//        }
//    }
//    
//    
//
//    if(touchedNode){
//    [touchedNode.physicsBody setAffectedByGravity:touchedNodeWasDynamic];
//    touchedNode = nil;
//    }
//}
//- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event{
//    if(touchedNode){
//    touchedNode.physicsBody.affectedByGravity = touchedNodeWasDynamic;
//    touchedNode = nil;
//    }
//}
//#else
//-(void)mouseDown:(NSEvent *)theEvent{
//    
//    CGPoint location = [theEvent locationInNode:self];
//    
//    ropeJointsCutStartPt = location;
//    NSArray* foundNodes = [self nodesAtPoint:location];
//    for(SKNode* foundNode in foundNodes)
//    {
//        if(foundNode.physicsBody){
//            touchedNode = foundNode;
//            touchedNodeWasDynamic = touchedNode.physicsBody.affectedByGravity;
//            [touchedNode.physicsBody setAffectedByGravity:NO];
//            break;
//        }
//    }
//    
//    BOOL                dragActive = YES;
//    NSEvent*            event = NULL;
//    NSWindow            *targetWindow = [[NSApplication sharedApplication] mainWindow];
//    
//    while (dragActive) {
//        event = [targetWindow nextEventMatchingMask:(NSLeftMouseDraggedMask | NSLeftMouseUpMask)
//                                          untilDate:[NSDate distantFuture]
//                                             inMode:NSEventTrackingRunLoopMode
//                                            dequeue:YES];
//        if(!event){
//            continue;
//        }
//        switch ([event type])
//        {
//            case NSLeftMouseDragged:
//            {
//                CGPoint curLocation = [event locationInNode:self];
//                
//                if(touchedNode && touchedNode.physicsBody){
//                    [touchedNode setPosition:curLocation];
//                }
//            }
//                break;
//                
//                
//            case NSLeftMouseUp:
//                dragActive = NO;
//                
//                CGPoint curLocation = [event locationInNode:self];
//                for(LHRopeJointNode* rope in ropeJoints){
//                    if([rope canBeCut]){
//                        [rope cutWithLineFromPointA:ropeJointsCutStartPt
//                                           toPointB:curLocation];
//                    }
//                }
//                
//                if(touchedNode){
//                    [touchedNode.physicsBody setAffectedByGravity:touchedNodeWasDynamic];
//                    touchedNode = nil;
//                }
//                
//                break;
//                
//            default:
//                break;
//        }
//    }
//}
//#endif

#pragma mark LHNodeProtocol Required

-(NSString*)uuid{
    return _uuid;
}

-(NSArray*)tags{
    return _tags;
}

-(id<LHUserPropertyProtocol>)userProperty{
    return _userProperty;
}

-(LHScene*)scene{
    return self;
}

-(CCNode <LHNodeProtocol>*)childNodeWithName:(NSString*)name{
    return [LHScene childNodeWithName:name
                              forNode:physicsNode];
}


-(CCNode <LHNodeProtocol>*)childNodeWithUUID:(NSString*)uuid{
    return [LHScene childNodeWithUUID:uuid
                              forNode:physicsNode];
}

-(NSMutableArray*)childrenWithTags:(NSArray*)tagValues
                       containsAny:(BOOL)any{
    return [LHScene childrenWithTags:tagValues
                         containsAny:any
                            forNode:physicsNode];
}


-(NSMutableArray*)childrenOfType:(Class)type{
    return [LHScene childrenOfType:type
                           forNode:physicsNode];
}

#pragma mark - TOUCH SUPPORT
-(void)touchBegan:(UITouch *)touch withEvent:(UIEvent *)event{

    //without this touch began is not called
    CCDirector* dir = [CCDirector sharedDirector];
    
    CGPoint touchLocation = [touch previousLocationInView: [touch view]];
	touchLocation = [dir convertToGL: touchLocation];
    
    ropeJointsCutStartPt = touchLocation;
}

-(void) touchMoved:(UITouch *)touch withEvent:(UIEvent *)event {
    
//    NSLog(@"TOUCH MOVED");
//    
////    NSLog(@"SELF CHILDREN %@", [self children]);
//    
//    CGPoint touchLoc = [touch locationInNode:self];
//    
//    CCDirector* dir = [CCDirector sharedDirector];
//    
//    CGPoint touchLocation = [touch previousLocationInView: [touch view]];
//	touchLocation = [dir convertToGL: touchLocation];
//    CGPoint previousLoc = [self convertToNodeSpace:touchLocation];
//    
//
//    CGPoint delta = CGPointMake(touchLoc.x - previousLoc.x,
//                                touchLoc.y - previousLoc.y);
//    
//    CGPoint curPos = [self position];
//    
//    [self setPosition:CGPointMake(curPos.x + delta.x, curPos.y + delta.y)];
//    
//    NSLog(@"NEW POS %f %f", self.position.x, self.position.y);
}

-(void)touchEnded:(UITouch *)touch withEvent:(UIEvent *)event
{
    CCDirector* dir = [CCDirector sharedDirector];
    
    CGPoint touchLocation = [touch previousLocationInView: [touch view]];
	touchLocation = [dir convertToGL: touchLocation];

    for(LHRopeJointNode* rope in [self childrenOfType:[LHRopeJointNode class]]){
        if([rope canBeCut]){
            [rope cutWithLineFromPointA:ropeJointsCutStartPt
                               toPointB:touchLocation];
        }
    }
}


@end


#pragma mark - PRIVATES

@implementation LHScene (LH_SCENE_NODES_PRIVATE_UTILS)

+(id)createLHNodeWithDictionary:(NSDictionary*)childInfo
                         parent:(CCNode*)prnt
{
    
    NSString* nodeType = [childInfo objectForKey:@"nodeType"];
    
    LHScene* scene = nil;
    if([prnt isKindOfClass:[LHScene class]]){
        scene = (LHScene*)prnt;
    }
    else if([[prnt scene] isKindOfClass:[LHScene class]]){
        scene = (LHScene*)[prnt scene];
    }

    
    if([nodeType isEqualToString:@"LHSprite"])
    {
        LHSprite* spr = [LHSprite spriteNodeWithDictionary:childInfo
                                                    parent:prnt];
        return spr;
    }
    else if([nodeType isEqualToString:@"LHNode"])
    {
        LHNode* nd = [LHNode nodeWithDictionary:childInfo
                                         parent:prnt];
        return nd;
    }
    else if([nodeType isEqualToString:@"LHBezier"])
    {
        LHBezier* bez = [LHBezier bezierNodeWithDictionary:childInfo
                                                    parent:prnt];
        return bez;
    }
    else if([nodeType isEqualToString:@"LHTexturedShape"])
    {
        LHShape* sp = [LHShape shapeNodeWithDictionary:childInfo
                                                parent:prnt];
        return sp;
    }
    else if([nodeType isEqualToString:@"LHWaves"])
    {
        LHWater* wt = [LHWater waterNodeWithDictionary:childInfo
                                                parent:prnt];
        return wt;
    }
    else if([nodeType isEqualToString:@"LHAreaGravity"])
    {
        LHGravityArea* gv = [LHGravityArea gravityAreaWithDictionary:childInfo
                                                              parent:prnt];
        return gv;
    }
    else if([nodeType isEqualToString:@"LHParallax"])
    {
        LHParallax* pr = [LHParallax parallaxWithDictionary:childInfo
                                                     parent:prnt];
        return pr;
    }
    else if([nodeType isEqualToString:@"LHParallaxLayer"])
    {
        LHParallaxLayer* lh = [LHParallaxLayer parallaxLayerWithDictionary:childInfo
                                                                    parent:prnt];
        return lh;
    }
    else if([nodeType isEqualToString:@"LHAsset"])
    {
        LHAsset* as = [LHAsset assetWithDictionary:childInfo
                                            parent:prnt];
        return as;
    }
    else if([nodeType isEqualToString:@"LHCamera"])
    {
        if(scene)
        {
            LHCamera* cm = [LHCamera cameraWithDictionary:childInfo
                                                    parent:prnt];
            return cm;
        }
    }
    else if([nodeType isEqualToString:@"LHRopeJoint"])
    {
        if(scene)
        {
            LHRopeJointNode* jt = [LHRopeJointNode ropeJointNodeWithDictionary:childInfo
                                                                        parent:prnt];
            [scene addLateLoadingNode:jt];
        }
    }
//    else if([nodeType isEqualToString:@"LHWeldJoint"])
//    {
//        LHWeldJointNode* jt = [LHWeldJointNode weldJointNodeWithDictionary:childInfo
//                                                                    parent:prnt];
//        [scene addDebugJointNode:jt];
//        [scene addLateLoadingNode:jt];
//    }
//    else if([nodeType isEqualToString:@"LHRevoluteJoint"]){
//        
//        LHRevoluteJointNode* jt = [LHRevoluteJointNode revoluteJointNodeWithDictionary:childInfo
//                                                                                parent:prnt];
//
//        [scene addDebugJointNode:jt];
//        [scene addLateLoadingNode:jt];
//    }
    else if([nodeType isEqualToString:@"LHDistanceJoint"]){
        
        LHDistanceJointNode* jt = [LHDistanceJointNode distanceJointNodeWithDictionary:childInfo
                                                                                parent:prnt];
        [scene addLateLoadingNode:jt];

    }
//    else if([nodeType isEqualToString:@"LHPrismaticJoint"]){
//        
//        LHPrismaticJointNode* jt = [LHPrismaticJointNode prismaticJointNodeWithDictionary:childInfo
//                                                                                   parent:prnt];
//        [scene addDebugJointNode:jt];
//        [scene addLateLoadingNode:jt];
//    }


    else{
        NSLog(@"UNKNOWN NODE TYPE %@", nodeType);
    }
    
    return nil;
}

+(CCNode <LHNodeProtocol>*)childNodeWithName:(NSString*)nameVal
                                     forNode:(CCNode*)selfNode{
    for(CCNode<LHNodeProtocol>* node in [selfNode children])
    {
        if([node conformsToProtocol:@protocol(LHNodeProtocol)])
        {
            if([[node name] isEqualToString:nameVal]){
                return node;
            }
            CCNode <LHNodeProtocol>* retNode = [node childNodeWithName:nameVal];
            if(retNode){
                return retNode;
            }
        }
    }
    return nil;
}

+(CCNode<LHNodeProtocol>*)childNodeWithUUID:(NSString*)uuid
                                    forNode:(CCNode*)selfNode{
    for(CCNode<LHNodeProtocol>* node in [selfNode children])
    {
        if([node conformsToProtocol:@protocol(LHNodeProtocol)])
        {
            if([[node uuid] isEqualToString:uuid]){
                return node;
            }
            
            if([node respondsToSelector:@selector(childNodeWithUUID:)])
            {
                CCNode<LHNodeProtocol>* retNode = [node childNodeWithUUID:uuid];
                if(retNode){
                    return retNode;
                }
            }
        }
    }
    return nil;
}

+(NSMutableArray*)childrenOfType:(Class)type
                         forNode:(CCNode*)selfNode{
    
    NSMutableArray* temp = [NSMutableArray array];
    for(CCNode* child in [selfNode children]){
        if([child isKindOfClass:type]){
            [temp addObject:child];
        }
        
        if([child respondsToSelector:@selector(childrenOfType:)])
        {
            NSMutableArray* childArray = [child performSelector:@selector(childrenOfType:)
                                          withObject:type];
            if(childArray){
                [temp addObjectsFromArray:childArray];
            }
        }
    }
    return temp;
}

+(NSMutableArray*)childrenWithTags:(NSArray*)tagValues
                       containsAny:(BOOL)any
                          forNode:(CCNode*)selfNode
{
    NSMutableArray* temp = [NSMutableArray array];
    for(id<LHNodeProtocol> child in [selfNode children]){
        if([child conformsToProtocol:@protocol(LHNodeProtocol)])
        {
            NSArray* childTags =[child tags];

            int foundCount = 0;
            BOOL foundAtLeastOne = NO;
            for(NSString* tg in childTags)
            {
                for(NSString* st in tagValues){
                    if([st isEqualToString:tg])
                    {
                        ++foundCount;
                        foundAtLeastOne = YES;
                        if(any){
                            break;
                        }
                    }
                }
                
                if(any && foundAtLeastOne){
                    [temp addObject:child];
                    break;
                }
            }
            if(!any && foundAtLeastOne && foundCount == [tagValues count] && [childTags count] == [tagValues count]){
                [temp addObject:child];
            }

            if([child respondsToSelector:@selector(childrenWithTags:containsAny:)])
            {
                NSMutableArray* childArray = [child childrenWithTags:tagValues containsAny:any];
                if(childArray){
                    [temp addObjectsFromArray:childArray];
                }
            }
        }
    }
    return temp;
}

-(NSString*)relativePath{
    return relativePath;
}

-(float)currentDeviceRatio{
    
#if TARGET_OS_IPHONE
    CGSize scrSize = LH_SCREEN_RESOLUTION;
#else
    CGSize scrSize = self.size;
#endif
    
    for(LHDevice* dev in supportedDevices){
        CGSize devSize = [dev size];
        if(CGSizeEqualToSize(scrSize, devSize)){
            return [dev ratio];
        }
    }
    return 1.0f;
}

-(CGSize)designResolutionSize{
    return designResolutionSize;
}
-(CGPoint)designOffset{
    return designOffset;
}

-(NSString*)currentDeviceSuffix:(BOOL)keep2x{
    
#if TARGET_OS_IPHONE
    CGSize scrSize = LH_SCREEN_RESOLUTION;
#else
    CGSize scrSize = self.size;
#endif
    
    for(LHDevice* dev in supportedDevices){
        CGSize devSize = [dev size];
        if(CGSizeEqualToSize(scrSize, devSize)){
            NSString* suffix = [dev suffix];
            if(!keep2x){
                suffix = [suffix stringByReplacingOccurrencesOfString:@"@2x"
                                                           withString:@""];
            }
            return suffix;
        }
    }
    return @"";
}
@end
 */

