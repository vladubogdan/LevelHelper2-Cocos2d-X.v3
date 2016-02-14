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
#include "LHBodyShape.h"


#include <sstream>
using namespace std;

#define kLHPinchThreshold 2.0

LHScene::LHScene()
{
#if LH_USE_BOX2D
    _box2dCollision = NULL;
    _physicsBoundarySubShapes = NULL;
#endif
    _currentDev = NULL;
    _touchListener = NULL;
    _loadedAssetsInformations = NULL;
    _tracedFixtures = NULL;
    _gameWorldNode = NULL;
    _lateLoadingNodes = NULL;
    _uiNode = NULL;
    _backUINode = NULL;
    loadingInProgress = false;
    
    lastDistanceBetweenTouches = 0.0f;
}

LHScene::~LHScene()
{
    _currentDev = nullptr;
    
#if LH_USE_BOX2D
    CC_SAFE_RELEASE(_physicsBoundarySubShapes);
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
    
    for (auto iter = _editorBodiesInfo.begin(); iter != _editorBodiesInfo.end(); ++iter)
    {
        std::map<std::string, Value* > obj = iter->second;
        for(auto subIter = obj.begin(); subIter != obj.end(); ++subIter)
        {
            Value* val = subIter->second;
            delete val;
        }
    }
    _editorBodiesInfo.clear();
    
    
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
    
    if(!FileUtils::getInstance()->isFileExist(fullPath))
    {
        printf("\nERROR: Could not load level file %s. The file located at %s doesn't exist.\n", plistLevelFile.c_str(), fullPath.c_str());
        return false;
    }
    
    LHDictionary* dict = (LHDictionary*)__Dictionary::createWithContentsOfFile(fullPath.c_str());
        
    if(!dict){
        printf("\nERROR: Could not load level file %s. The file located at %s does not appear to be valid.\n", plistLevelFile.c_str(), fullPath.c_str());
        return false;
    }


    int aspect = dict->intForKey("aspect");
    
    
    designResolutionSize = dict->sizeForKey("designResolution");
    LHArray* devsInfo = dict->arrayForKey("devices");
    
    if(!devsInfo){
        printf("\nERROR: Level doesn't contain valid devices.\n");
        return false;
    }
    
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
        loadingInProgress = true;
        
        _gameWorldNode = NULL;
        _uiNode = NULL;
        _backUINode = NULL;
        
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

        Director::getInstance()->setClearColor(Color4F(backgroundClr.r/255.0f, backgroundClr.g/255.0f, backgroundClr.b/255.0f, 1.0f));
        
        
        this->loadGameWorldInfoFromDictionary(dict);
        
        this->loadChildrenFromDictionary(dict);
        
#if LH_DEBUG
        getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif
        
        this->loadGlobalGravityFromDictionary(dict);
        this->loadPhysicsBoundariesFromDictionary(dict);

        this->performLateLoading();
        
        
#if LH_USE_BOX2D
        _box2dCollision = new LHBox2dCollisionHandling(this);
#else//cocos2d-x

        
#endif
        
//        auto contactListener = EventListenerPhysicsContact::create();
//        contactListener->onContactBegin = CC_CALLBACK_1(LHScene::onContactBegin, this);
//        _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
        
        loadingInProgress = false;
        
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

void LHScene::loadGameWorldInfoFromDictionary(LHDictionary* dict)
{
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
        }
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
                                                 "LHPhysicsTopBoundary");
            }
            
            {
                createPhysicsBoundarySectionFrom(Point(skBRect.getMaxX(), skBRect.getMinY()),
                                                 Point(skBRect.getMaxX(), skBRect.getMaxY()),
                                                 "LHPhysicsRightBoundary");
                
            }
            
            {
                createPhysicsBoundarySectionFrom(Point(skBRect.getMaxX(), skBRect.getMaxY()),
                                                 Point(skBRect.getMinX(), skBRect.getMaxY()),
                                                 "LHPhysicsBottomBoundary");
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
    LHNode* drawNode = LHNode::createWithName(sectionName);
    
#if LH_USE_BOX2D
    
    if(!_physicsBoundarySubShapes){
        _physicsBoundarySubShapes = new __Array();
        _physicsBoundarySubShapes->init();
    }
    
    LHBodyShape* shape = LHBodyShape::createWithName(sectionName, from, to, drawNode, this);
    _physicsBoundarySubShapes->addObject(shape);
    
#else //chipmunk
    PhysicsBody* boundaryBody = PhysicsBody::createEdgeSegment(from, to);
    drawNode->setPhysicsBody(boundaryBody);
#endif
    
    this->getGameWorldNode()->addChild(drawNode);
}


// on "init" you need to initialize your instance
void LHScene::onEnter()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();

    listener->onTouchesBegan = CC_CALLBACK_2(LHScene::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(LHScene::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(LHScene::onTouchesEnded, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(LHScene::onTouchesCancelled, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
  
    //multi touch on iOS will only work if you call this inside AppController.mm
    //[eaglView setMultipleTouchEnabled:YES];
    
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseScroll = CC_CALLBACK_1(LHScene::onMouseScroll, this);
    dispatcher->addEventListenerWithFixedPriority(mouseListener, 1);
    _mouseListener = mouseListener;
    
    Scene::onEnter();
}


void LHScene::onExit()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    _touchListener = nullptr;
    
    dispatcher->removeEventListener(_mouseListener);
    _mouseListener = nullptr;
    
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

bool LHScene::hasEditorBodyInfoForImageFilePath(const std::string& imageFilePath)
{
    return _editorBodiesInfo.find( imageFilePath ) != _editorBodiesInfo.end();
}

Value* LHScene::getEditorBodyInfoForSpriteName(const std::string& spriteName, const std::string& imageFilePath)
{
    std::map<std::string, Value*>spritesInfo = _editorBodiesInfo[imageFilePath];
    return spritesInfo[spriteName];
}
void LHScene::setEditorBodyInfoForSpriteName(const std::string& spriteName, const std::string& imageFilePath, Value& bodyInfo)
{
    std::map<std::string, Value*> spritesInfo;
    if(_editorBodiesInfo.find( imageFilePath ) != _editorBodiesInfo.end())
    {
        spritesInfo = _editorBodiesInfo[imageFilePath];
    }
    
    if(!bodyInfo.isNull() && spritesInfo.find(spriteName) == spritesInfo.end())
    {
        spritesInfo[spriteName] = new Value(bodyInfo);
    }
    _editorBodiesInfo[imageFilePath] = spritesInfo;
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
bool LHScene::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}
void LHScene::onTouchMoved(Touch* touch, Event* event)
{
    
}
void LHScene::onTouchEnded(Touch* touch, Event* event)
{
    
}
void LHScene::onTouchCancelled(Touch *touch, Event *event)
{
    
}

void LHScene::onTouchesBegan(const std::vector<Touch*>& touches, Event* event){
    
    if(touches.size() < 1){
        return;
    }
    
    Touch* touch = touches[0];
    _ropeJointsCutStartPt = touch->getLocation();
    
    onTouchBegan(touch, event);//XXX Soon to be removed
}


void LHScene::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
    if(touches.size() < 1){
        return;
    }
    
    Touch * touch1 = (Touch*)touches[0];
    onTouchMoved(touch1, event);//XXX Soon to be removed
    
    if(touches.size() < 2)return;
    
    Touch * touch2 = (Touch*)touches[1];
    Point delta1 = touch1->getDelta();
    Point delta2 = touch2->getDelta();
    
    if ((fabs(delta1.x)<kLHPinchThreshold && fabs(delta1.y)<kLHPinchThreshold) ||
        (fabs(delta2.x)<kLHPinchThreshold && fabs(delta2.y)<kLHPinchThreshold)) {
        return;
    }
    
    Point p1 = touch1->getLocation();
    Point p2 = touch2->getLocation();
    float deltaX = p2.x-p1.x;
    float deltaY = p2.y-p1.y;
    float distance = fabs(sqrtf(deltaX*deltaX+deltaY*deltaY));
    
    if (!lastDistanceBetweenTouches) {
        lastDistanceBetweenTouches = distance;
        return;
    }

    bool close = false;
    if (lastDistanceBetweenTouches >distance) {
        close = true;
    }

    if ((delta1.x>0 && delta2.x<0) || (delta1.x<0 && delta2.x>0) ||
        (delta1.y>0 && delta2.y<0) || (delta1.y<0 && delta2.y>0))
    {
        float delta = distance - lastDistanceBetweenTouches;
        
        Point location1 = touch1->getLocation();
        Point location2 = touch2->getLocation();
        Point center = Point((location1.x + location2.x)*0.5, (location1.y + location2.y)*0.5);
        
        this->didPinchAtLocation(center, delta, close);
    }

    lastDistanceBetweenTouches = distance;
}
void LHScene::didPinchAtLocation(Point centerTouch, float delta, bool closing)
{
    //CCLOG("DID PINCH AT %f %f delta %f close %d", centerTouch.x, centerTouch.y, delta, closing);
    
    centerTouch = this->convertToNodeSpace(centerTouch);
    
    __Array* cameras = this->getChildrenOfType<LHCamera*>();
    
    for(size_t i = 0; i< cameras->count(); ++i)
    {
        LHCamera* cam = (LHCamera*)cameras->getObjectAtIndex(i);
        
        if(cam->isActive() && cam->usePinchOrScrollWheelToZoom()){
            
            float zoomDelta = 0.2;
            if(delta< 0){
                zoomDelta = -0.2;
            }
            cam->pinchZoomWithScaleDeltaAndCenter(zoomDelta, centerTouch);
        }
    }
}

void LHScene::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    if(touches.size() <= 0){
        return;
    }
    
    Touch* touch = touches[0];
    Point location = touch->getLocation();
    
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
    
    onTouchEnded(touch, event);//XXX Soon to be removed
}

void LHScene::onTouchesCancelled(const std::vector<Touch*>& touches, Event *event)
{
    if(touches.size() <= 0){
        return;
    }
    
    Touch* touch = touches[0];
    onTouchCancelled(touch, event);//XXX Soon to be removed
}

void LHScene::onMouseScroll(Event* event){
    
    if(event->getType() == Event::Type::MOUSE)
    {
        EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
        
        float deltaY = mouseEvent->getScrollY();
        
        float x = mouseEvent->getCursorX();
        float y = mouseEvent->getCursorY();
        
        Point location = this->convertToNodeSpace(Point(x,y));

        __Array* cameras = this->getChildrenOfType<LHCamera*>();
  
        for(size_t i = 0; i< cameras->count(); ++i)
        {
            LHCamera* cam = (LHCamera*)cameras->getObjectAtIndex(i);
            
            if(cam->isActive() && cam->usePinchOrScrollWheelToZoom()){
                
                float newScale = _gameWorldNode->getScale();
                if(deltaY > 0)
                    newScale += 0.025*_gameWorldNode->getScale();
                else if(deltaY <0)
                    newScale -= 0.025*_gameWorldNode->getScale();
                
                float delta = newScale - _gameWorldNode->getScale();
                
                cam->pinchZoomWithScaleDeltaAndCenter(delta, location);
            }
        }
    }
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

