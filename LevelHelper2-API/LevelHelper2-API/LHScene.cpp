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
#include "LHConfig.h"


#include "LHSprite.h"
#include "LHNode.h"
#include "LHCamera.h"
#include "LHBezier.h"
#include "LHShape.h"
#include "LHAsset.h"
#include "LHParallax.h"
#include "LHParallaxLayer.h"
#include "LHRopeJointNode.h"

#include "LHGameWorldNode.h"
#include "LHUINode.h"
#include "LHBackUINode.h"
#include "LHWater.h"
#include "LHUtils.h"
#include "LHBox2dCollisionHandling.h"


#include <sstream>
using namespace std;


using namespace cocos2d;

LHScene::LHScene()
{
    _currentDev = nullptr;
    _touchListener = nullptr;
    _loadedAssetsInformations = nullptr;
    _tracedFixtures = nullptr;
    _gameWorldNode = nullptr;
    _lateLoadingNodes = nullptr;
    _uiNode = nullptr;
    _backUINode = nullptr;
    
//    printf("lhscene constructor\n");
}

LHScene::~LHScene()
{
    _currentDev = nullptr;
    
#if LH_USE_BOX2D
    CC_SAFE_DELETE(_box2dCollision);
#endif
    
    for (size_t i = 0; i < devices.size(); ++i)
    {
        LHDevice* dev = devices[i];
        delete dev;
        devices[i] = NULL;
    }
    devices.clear();
    
    CC_SAFE_RELEASE(_tracedFixtures);
    _gameWorldNode = nullptr;
    _uiNode = nullptr;
    _backUINode = nullptr;

    CC_SAFE_DELETE(_loadedAssetsInformations);
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


bool LHScene::initWithContentOfFile(const std::string& plistLevelFile)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(plistLevelFile);
    
    LHDictionary* dict = (LHDictionary*)__Dictionary::createWithContentsOfFile(fullPath.c_str());
    
    if(!dict){
        printf("ERROR: Could not load level file %s. The file located at %s does not appear to be valid.", plistLevelFile.c_str(), fullPath.c_str());
        return false;
    }

    
    
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
        _currentDev = curDev;
    
        //this can cause issues with transitioning to new scenes.
//        It is user responsibility to do this anyway also if the next level has the same textures - loading will be faster
//        SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
//        Director::getInstance()->getTextureCache()->removeUnusedTextures();
        
        this->loadGenericInfoFromDictionary(dict);
        
        setContentSize(sceneSize);
        this->setPosition(Point());
        this->setName(plistLevelFile);
        
        LHDictionary* tracedFixInfo = dict->dictForKey("tracedFixtures");
        if(tracedFixInfo){
            _tracedFixtures = __Dictionary::createWithDictionary(tracedFixInfo);
            _tracedFixtures->retain();
        }

        //load background color
        Color3B backgroundClr = dict->colorForKey("backgroundColor");
        glClearColor(backgroundClr.r/255.0f, backgroundClr.g/255.0f, backgroundClr.b/255.0f, 1.0f);

        
        this->loadChildrenFromDictionary(dict);
        
#if LH_DEBUG
        getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif
        
        
        this->loadGlobalGravityFromDictionary(dict);        
        this->loadPhysicsBoundariesFromDictionary(dict);
        
        
        
        
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
        
        
        
        
        
        
        
        this->performLateLoading();
        
        
#if LH_USE_BOX2D
        _box2dCollision = new LHBox2dCollisionHandling(this);
#else//cocos2d-x

        
#endif
        
//        auto contactListener = EventListenerPhysicsContact::create();
//        contactListener->onContactBegin = CC_CALLBACK_1(LHScene::onContactBegin, this);
//        _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
        
        ret = true;
    };
    
    return ret;
}

void LHScene::loadGlobalGravityFromDictionary(LHDictionary* dict)
{
    if(dict->boolForKey("useGlobalGravity"))
    {
        Point gravityVector = dict->pointForKey("globalGravityDirection");
        float gravityForce  = dict->floatForKey("globalGravityForce");
        
        Point gravity = Point(gravityVector.x*gravityForce,
                              gravityVector.y*gravityForce);

        
#if LH_USE_BOX2D
        this->setGlobalGravity(gravity);
#else//chipmunk
        this->getPhysicsWorld()->setGravity(Point(gravityVector.x*gravityForce*100,
                                                  gravityVector.y*gravityForce*100));
#endif
    }
}

void LHScene::loadPhysicsBoundariesFromDictionary(LHDictionary* dict)
{
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
                         designSize.height -  bRect.origin.y*designSize.height + offset.y,
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
}

void LHScene::createPhysicsBoundarySectionFrom(Point from, Point to, const std::string& sectionName)
{
#if LH_USE_BOX2D
    
    float PTM_RATIO = this->getPtm();
    
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner
    
    b2Body* physicsBoundariesBody = this->getBox2dWorld()->CreateBody(&groundBodyDef);
    
    // Define the ground box shape.
    b2EdgeShape groundBox;
    
    // top
    groundBox.Set(b2Vec2(from.x/PTM_RATIO,
                         from.y/PTM_RATIO),
                  b2Vec2(to.x/PTM_RATIO,
                         to.y/PTM_RATIO));
    physicsBoundariesBody->CreateFixture(&groundBox,0);
    
#else //chipmunk
    LHNode* drawNode = LHNode::createWithName(sectionName);
    PhysicsBody* boundaryBody = PhysicsBody::createEdgeSegment(from, to);
    drawNode->setPhysicsBody(boundaryBody);
    this->getGameWorldNode()->addChild(drawNode);
#endif
    
}


// on "init" you need to initialize your instance
void LHScene::onEnter()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(LHScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LHScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LHScene::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(LHScene::onTouchCancelled, this);
    dispatcher->addEventListenerWithFixedPriority(listener, 1);
    _touchListener = listener;
    
    Scene::onEnter();
}


void LHScene::onExit()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    _touchListener = nullptr;
    
    Scene::onExit();
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


__Array* LHScene::tracedFixturesWithUUID(const std::string& uuid)
{
    return (__Array*)_tracedFixtures->objectForKey(uuid);
}

std::string LHScene::getCurrentDeviceSuffix()
{
    if(_currentDev){
        return _currentDev->getSuffix();
    }
    
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

LHBackUINode* LHScene::getBackUINode(){
    if(!_backUINode){
        __Array* children = this->getChildrenOfType<LHBackUINode*>();
        if(children->count() > 0){
            _backUINode = (LHBackUINode*)children->getObjectAtIndex(0);
        }
    }
    return _backUINode;
}
LHGameWorldNode* LHScene::getGameWorldNode(){
    if(!_gameWorldNode){
        __Array* children = this->getChildrenOfType<LHGameWorldNode*>();
        if(children->count() > 0){
            _gameWorldNode = (LHGameWorldNode*)children->getObjectAtIndex(0);
        }
    }
    return _gameWorldNode;
}
LHUINode* LHScene::getUINode(){
    if(!_uiNode){
        __Array* children = this->getChildrenOfType<LHUINode*>();
        if(children->count() > 0){
            _uiNode = (LHUINode*)children->getObjectAtIndex(0);
        }
    }
    return _uiNode;
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





#pragma mark - TOUCHES
bool LHScene::onTouchBegan(Touch* touch, Event* event){
    
    _ropeJointsCutStartPt = touch->getLocation();
    return true;
}
void LHScene::onTouchMoved(Touch* touch, Event* event){
    
}
void LHScene::onTouchEnded(Touch* touch, Event* event){
 
    Point location = touch->getLocation();
    
//    CCLOG("TOUCH ENDED %f %f", location.x, location.y);
    
    __Array* ropeJoints = this->getChildrenOfType<LHRopeJointNode*>();
    
    for(size_t i = 0; i< ropeJoints->count(); ++i)
    {
        LHRopeJointNode* rope = (LHRopeJointNode*)ropeJoints->getObjectAtIndex(i);

        if(rope->canBeCut())
        {
            rope->cutWithLineFromPointA(_ropeJointsCutStartPt,
                                        location);
        }
    }
}
void LHScene::onTouchCancelled(Touch *touch, Event *event){

}

#pragma mark - BOX2D SUPPORT
#if LH_USE_BOX2D

b2World* LHScene::getBox2dWorld(){
    return this->getGameWorldNode()->getBox2dWorld();
}
float LHScene::getPtm(){
    return 32.0f;
}
b2Vec2 LHScene::metersFromPoint(Point point){
    return b2Vec2(point.x/this->getPtm(), point.y/this->getPtm());
}
Point LHScene::pointFromMeters(b2Vec2 vec){
    return Point(vec.x*this->getPtm(), vec.y*this->getPtm());
}

float LHScene::metersFromValue(float val){
    return val/this->getPtm();
}
float LHScene::valueFromMeters(float meter){
    return meter*this->getPtm();
}

void LHScene::setBox2dFixedTimeStep(float val){
    this->getGameWorldNode()->setBox2dFixedTimeStep(val);
}
void LHScene::setBox2dMinimumTimeStep(float val){
    this->getGameWorldNode()->setBox2dMinimumTimeStep(val);
}
void LHScene::setBox2dVelocityIterations(int val){
    this->getGameWorldNode()->setBox2dVelocityIterations(val);
}
void LHScene::setBox2dPositionIterations(int val){
    this->getGameWorldNode()->setBox2dPositionIterations(val);
}
void LHScene::setBox2dMaxSteps(int val){
    this->getGameWorldNode()->setBox2dMaxSteps(val);
}

#endif //LH_USE_BOX2D


Point LHScene::getGlobalGravity()
{
#if LH_USE_BOX2D
    return this->getGameWorldNode()->getGravity();
#else
    return this->getPhysicsWorld()->getGravity();
#endif
}
/*Sets the global gravity force
 @param gravity A point representing the gravity force in x and y direction.
 */
void LHScene::setGlobalGravity(Point gravity)
{
#if LH_USE_BOX2D
    this->getGameWorldNode()->setGravity(gravity);
#else
    this->getPhysicsWorld()->setGravity(gravity);
#endif
}


//#pragma mark - COLLISION HANDLING
//
//#if LH_USE_BOX2D
//
//#else
//
//bool LHScene::onContactBegin(PhysicsContact& contact)
//{
//    CCLOG("LHSCENE CONTACT BEGIN");
//    
//    return true;
//    //nothing to do - should be overwritten
//}
//int LHScene::collisionPreSolveCallback(PhysicsContact& contact){
//    //nothing to do - should be overwritten
//}
//void LHScene::collisionPostSolveCallback(PhysicsContact& contact){
//    //nothing to do - should be overwritten
//}
//void LHScene::collisionSeparateCallback(PhysicsContact& contact){
//    //nothing to do - should be overwritten
//}
//#endif


