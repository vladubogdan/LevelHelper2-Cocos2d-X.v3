//
//  LHCamera.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHCamera.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHDevice.h"
#include "LHNode.h"
#include "LHGameWorldNode.h"
#include "LHUtils.h"

LHCamera::LHCamera()
{
    wasUpdated= false;
    _followedNode = nullptr;
    
    offset = Point();
    importantArea = Size();
    
    lockX = false;
    lockY = false;
    smoothMovement = false;
    
    zooming = false;
    startZoomValue = 1.0f;
    reachZoomValue = 1.0f;
    reachZoomTime = 0.0f;
    minZoomValue = 0.01f;
    zoomStartTime = 0;
    
    lookingAt = false;
    resetingLookAt = false;
    _lookAtNode = nullptr;
    
    lookAtTime = 0;
    
    _zoomsOnPinch = false;
    
    reachingOffsetX = false;
    reachingOffsetY = false;
    
    directionMultiplierY = 1.0f;
    directionMultiplierX = 1.0f;
    previousFollowedPosition = Point();
    previousDirectionVector = Point();
}

LHCamera::~LHCamera()
{
    _followedNode = nullptr;
    _lookAtNode = nullptr;
}

LHCamera* LHCamera::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHCamera *ret = new LHCamera();
    if (ret && ret->initWithDictionary(dict, prnt))
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

bool LHCamera::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        wasUpdated = false;
        
        _physicsBody = NULL;
        prnt->addChild(this);
        
        this->loadGenericInfoFromDictionary(dict);
        this->loadTransformationInfoFromDictionary(dict);
        
        if(dict->objectForKey("followedNodeUUID")){
            _followedNodeUUID = dict->stringForKey("followedNodeUUID");
        }
        
        _active     = dict->boolForKey("activeCamera");
        _restricted = dict->boolForKey("restrictToGameWorld");
        
        this->createAnimationsFromDictionary(dict);
        
        LHScene* scene = (LHScene*)this->getScene();
        
        Point newPos = this->getPosition();
        Size winSize = scene->getContentSize();;
        newPos = Point(winSize.width*0.5  - newPos.x,
                       winSize.height*0.5 - newPos.y);
        
        Node::setPosition(this->transformToRestrictivePosition(newPos));
        
        Rect worldRect = scene->getGameWorldRect();
        
        Size worldSize = worldRect.size;
        if(worldSize.width < 0)
            worldSize.width = -worldSize.width;
        
        if(worldSize.height < 0)
            worldSize.height = -worldSize.height;
        
        minZoomValue = 0.1;
        if(_restricted){
            if(winSize.width < worldSize.width || winSize.height < worldSize.height){                
                minZoomValue = winSize.height/worldSize.height;
                if(minZoomValue < winSize.width/worldSize.width){
                    minZoomValue = winSize.width/worldSize.width;
                }
            }
        }
        
        if(dict->objectForKey("offset"))//all this properties were added at the same time
        {
            _zoomsOnPinch = dict->boolForKey("zoomOnPinchOrScroll");
            
            lockX = dict->boolForKey("lockX");
            lockY = dict->boolForKey("lockY");
            importantArea = dict->sizeForKey("importantArea");
            smoothMovement = dict->boolForKey("smoothMovement");
            offset = dict->pointForKey("offset");
            
            float zoomVal = dict->floatForKey("zoomValue");
            this->setZoomValue(zoomVal);
        }
        
        return true;
    }
    
    return false;
}

bool LHCamera::isActive(){
    return _active;
}
void LHCamera::resetActiveState(){
    _active = false;
}
void LHCamera::setActive(bool value){
    __Array* cameras = ((LHScene*)getScene())->getChildrenOfType<LHCamera*>();
    for(int i = 0; i < cameras->count(); ++i)
    {
        LHCamera* cam = (LHCamera*)cameras->getObjectAtIndex(i);
        cam->resetActiveState();
    }
    _active = value;
    setSceneView();
}

Node* LHCamera::followedNode()
{
    if(_followedNode == nullptr && _followedNodeUUID.length()> 0){
        _followedNode = ((LHScene*)getScene())->getGameWorldNode()->getChildNodeWithUUID(_followedNodeUUID);
        if(_followedNode){
            _followedNodeUUID ="";
        }
    }
    return _followedNode;
}
void LHCamera::followNode(Node* node){
    _followedNode = node;
}

bool LHCamera::restrictedToGameWorld(){
    return _restricted;
}
void LHCamera::setRestrictedToGameWorld(bool val){
    _restricted = val;
}

void LHCamera::setPosition(Point position){
    
    if(_active){
        
        Point transPoint = this->transformToRestrictivePosition(position);
        
        if(lockX){
            transPoint.x = position.x;
        }
        if(lockY){
            transPoint.y = position.y;
        }

        Node::setPosition(transPoint);
    }
    else{
        Node::setPosition(position);
    }
}

void LHCamera::setOffsetUnit(const Point& val){
    offset = val;
}

Point LHCamera::getOffsetUnit(){
    return offset;
}

void LHCamera::setImportantAreaUnit(const Size& unit){
    importantArea = unit;
}

Size LHCamera::getImportantAreaUnit(){
    return importantArea;
}

void LHCamera::setLockX(bool val){
    lockX = val;
}

bool LHCamera::getLockX(){
    return lockX;
}

void LHCamera::setLockY(bool val){
    lockY = val;
}

bool LHCamera::getLockY(){
    return lockY;
}

void LHCamera::setSmoothMovement(bool val){
    smoothMovement = val;
}

bool LHCamera::getSmoothMovement(){
    return smoothMovement;
}

void LHCamera::zoomByValueInSeconds(float value, float seconds){
    
    if(_active)
    {
        zooming = true;
        reachZoomTime = seconds;
        startZoomValue = ((LHScene*)getScene())->getGameWorldNode()->getScale();
        reachZoomValue = value + startZoomValue;
        
        if(reachZoomValue < minZoomValue){
            reachZoomValue = minZoomValue;
        }
        zoomStartTime = LHUtils::LHMillisecondNow();
    }
    
}

void LHCamera::zoomToValueInSeconds(float value, float seconds){
    
    if(_active)
    {
        zooming = true;
        reachZoomTime = seconds;
        startZoomValue = ((LHScene*)getScene())->getGameWorldNode()->getScale();
        reachZoomValue = value;
        
        if(reachZoomValue < minZoomValue){
            reachZoomValue = minZoomValue;
        }
        zoomStartTime = LHUtils::LHMillisecondNow();
    }
    
}

float LHCamera::getZoomValue(){
    return ((LHScene*)getScene())->getGameWorldNode()->getScale();
}

void LHCamera::setZoomValue(float value){
    
    Point transPoint = this->transformToRestrictivePosition(this->getPosition());
    LHGameWorldNode* gwNode = ((LHScene*)this->getScene())->getGameWorldNode();
    gwNode->setScale(value);
    gwNode->setPosition(transPoint);
    
}

void LHCamera::lookAtPositionInSeconds(const Point& gwPosition, float seconds){
    
    if(lookingAt == true){
        CCLOG("Camera is already looking somewhere. Please first reset lookAt by calling resetLookAt");
        return;
    }
    
    lookAtPosition = gwPosition;
    startLookAtPosition = _viewPosition;
    
    lookAtStartTime = LHUtils::LHMillisecondNow();
    lookAtTime = seconds;
    lookingAt = true;
}

void LHCamera::lookAtNodeInSeconds(Node* node, float seconds){
    
    if(lookingAt == true){
        CCLOG("Camera is already looking somewhere. Please first reset lookAt by calling resetLookAt");
        return;
    }
    
    _lookAtNode = node;
    
    startLookAtPosition = _viewPosition;
    
    lookAtStartTime = LHUtils::LHMillisecondNow();
    lookAtTime = seconds;
    lookingAt = true;
}

void LHCamera::resetLookAt(){
    this->resetLookAtInSeconds(0);
}

void LHCamera::resetLookAtInSeconds(float seconds){
    
    if(lookingAt != true){
        CCLOG("[ lookAtPosition: inSeconds:] must be used first. Cannot reset camera look.");
        return;
    }
    
    startLookAtPosition = lookAtPosition;
    
    if(_lookAtNode)
    {
        LHGameWorldNode* gwNode = ((LHScene*)this->getScene())->getGameWorldNode();
        
        Point worldPoint = _lookAtNode->convertToWorldSpaceAR(Point());
        startLookAtPosition = gwNode->convertToNodeSpaceAR(worldPoint);
        _lookAtNode = nullptr;
    }
    
    lookAtPosition = _centerPosition;
    
    lookAtStartTime = LHUtils::LHMillisecondNow();
    lookAtTime = seconds;
    lookingAt = true;
    resetingLookAt = true;
    
}

bool LHCamera::isLookingAt(){
    return lookingAt;
}

void LHCamera::setUsePinchOrScrollWheelToZoom(bool value){
    _zoomsOnPinch = value;
}

bool LHCamera::usePinchOrScrollWheelToZoom(){
    return _zoomsOnPinch;
}

void LHCamera::pinchZoomWithScaleDeltaAndCenter(float delta, const Point& scaleCenter)
{
    LHGameWorldNode* gwNode = ((LHScene*)this->getScene())->getGameWorldNode();
    
    float newScale = gwNode->getScale() + delta;
    
    if(newScale < minZoomValue){
        newScale = minZoomValue;
    }
    
    Point sclCenter = gwNode->convertToNodeSpaceAR(scaleCenter);
    
    Point oldCenterPoint = Point(sclCenter.x * gwNode->getScale(), sclCenter.y * gwNode->getScale());
    gwNode->setScale(newScale);
    Point newCenterPoint = Point(sclCenter.x * gwNode->getScale(), sclCenter.y * gwNode->getScale());
    
    Point centerPointDelta = oldCenterPoint - newCenterPoint;// ccpSub(oldCenterPoint, newCenterPoint);
    this->setPosition(this->getPosition() + centerPointDelta);// ccpAdd(self.position, centerPointDelta);
    
    this->performVisit();
}


void LHCamera::setSceneView(){

    if(_active)
    {
        Point transPoint = this->transformToRestrictivePosition(this->getPosition());
        
        LHGameWorldNode* gwNode = ((LHScene*)this->getScene())->getGameWorldNode();
        
        if(zooming)
        {
            unsigned long long currentTimer = LHUtils::LHMillisecondNow();
            float zoomUnit = (currentTimer - zoomStartTime)/reachZoomTime;
            float deltaZoom = startZoomValue + (reachZoomValue - startZoomValue)*zoomUnit;
            
            if(reachZoomValue < minZoomValue){
                reachZoomValue = minZoomValue;
            }
            
            gwNode->setScale(deltaZoom);
            
            if(zoomUnit >= 1.0f){
                gwNode->setScale(reachZoomValue);
                zooming = false;
            }
        }
        
        gwNode->setPosition(transPoint);
    }
}

Point LHCamera::transformToRestrictivePosition(Point position)
{
    LHScene* scene = (LHScene*)this->getScene();
    LHGameWorldNode* gwNode = scene->getGameWorldNode();

    Point transPoint = position;
    
    _viewPosition = transPoint;
    _centerPosition = transPoint;
    
    Size winSize = scene->getContentSize();
    Point halfWinSize = Point(winSize.width * 0.5f, winSize.height * 0.5f);
    
    Node* followed = this->followedNode();
    if(followed){
        
        Point gwNodePos = followed->getPosition();
        
        if(followed->getParent() != gwNode)
        {
            Point worldPoint = followed->convertToWorldSpaceAR(Point());
            gwNodePos = gwNode->convertToNodeSpaceAR(worldPoint);
        }
        
        _viewPosition = gwNodePos;
        _centerPosition = transPoint;
        
        Point scaledMidpoint = gwNodePos* gwNode->getScale();// ccpMult(gwNodePos, gwNode.scale);
        Point followedPos = halfWinSize - scaledMidpoint;// ccpSub(halfWinSize, scaledMidpoint);
        
        
        if(!lockX){
            transPoint.x = followedPos.x;
            transPoint.x -= directionalOffset.x;
        }
        if(!lockY){
            transPoint.y = followedPos.y;
            transPoint.y -= directionalOffset.y;
        }
        
        transPoint.x += offset.x*winSize.width;
        transPoint.y += offset.y*winSize.height;
    }
    
    
    unsigned long long currentTimer = LHUtils::LHMillisecondNow();
    float lookAtUnit = (currentTimer - lookAtStartTime)/lookAtTime;
    
    if(lookingAt)
    {
        if(_lookAtNode)
        {
            Point worldPoint = _lookAtNode->convertToWorldSpaceAR(Point());
            lookAtPosition = gwNode->convertToNodeSpaceAR(worldPoint);
        }
        
        float newX = startLookAtPosition.x + (lookAtPosition.x - startLookAtPosition.x)*lookAtUnit;
        float newY = startLookAtPosition.y + (lookAtPosition.y - startLookAtPosition.y)*lookAtUnit;
        Point gwNodePos(newX, newY);
        
        if(lookAtUnit >= 1.0f){
            gwNodePos = lookAtPosition;
        }
        
        _viewPosition = gwNodePos;
        
        Point scaledMidpoint = gwNodePos* gwNode->getScale();//ccpMult(gwNodePos, gwNode.scale);
        transPoint = halfWinSize - scaledMidpoint;// ccpSub(halfWinSize, scaledMidpoint);
    }
    
    if(resetingLookAt)
    {
        float newX = startLookAtPosition.x + (_centerPosition.x - startLookAtPosition.x)*lookAtUnit;
        float newY = startLookAtPosition.y + (_centerPosition.y - startLookAtPosition.y)*lookAtUnit;
        Point gwNodePos(newX, newY);
        
        if(lookAtUnit >= 1.0f){
            gwNodePos = lookAtPosition;
            resetingLookAt = false;
            lookingAt = false;
            _lookAtNode = nullptr;
        }
        
        _viewPosition = gwNodePos;
        
        Point scaledMidpoint = gwNodePos * gwNode->getScale();// ccpMult(gwNodePos, gwNode.scale);
        transPoint = halfWinSize - scaledMidpoint;// ccpSub(halfWinSize, scaledMidpoint);
    }
    
    
    float x = transPoint.x;
    float y = transPoint.y;
    
    Rect worldRect = scene->getGameWorldRect();
    
    worldRect.origin.x *= gwNode->getScale();
    worldRect.origin.y *= gwNode->getScale();
    worldRect.size.width *= gwNode->getScale();
    worldRect.size.height *= gwNode->getScale();
    
    if(!worldRect.equals(Rect()) && this->restrictedToGameWorld()){

        x = MAX(x, winSize.width*0.5 - (worldRect.origin.x + worldRect.size.width - winSize.width *0.5));
        x = MIN(x, winSize.width*0.5 - (worldRect.origin.x + winSize.width *0.5));
        
        y = MIN(y, winSize.height*0.5 - (worldRect.origin.y + worldRect.size.height + (winSize.height*0.5)));
        y = MAX(y, winSize.height*0.5 - (worldRect.origin.y - winSize.height*0.5));
    }
    
    transPoint.x = x;
    transPoint.y = y;
    
    return transPoint;
}

void LHCamera::performVisit()
{
    if(!isActive())return;
    
    
    Node* followed = this->followedNode();
    if(followed){
        
        LHScene* scene = (LHScene*)this->getScene();
        Size winSize = scene->getContentSize();
        
        Point curPosition = followed->getPosition();
        
        if(previousFollowedPosition.equals(Point())){
            previousFollowedPosition = curPosition;
            
            directionalOffset.x = -importantArea.width * winSize.width * 0.5;
            directionalOffset.y = importantArea.height * winSize.height * 0.5;
        }
        
        if(!curPosition.equals(previousFollowedPosition))
        {
            Point direction = curPosition - previousFollowedPosition;// ccpSub(curPosition,previousFollowedPosition);
            
            if(previousFollowedPosition.equals(Point())){
                previousDirectionVector = direction;
            }
            
            float followedDeltaX = curPosition.x - previousFollowedPosition.x;
            float followedDeltaY = curPosition.y - previousFollowedPosition.y;
            
            float filteringFactor = 0.50;
            
            
            if(reachingOffsetX)
            {
                float lastOffset = directionalOffset.x;
                
                directionalOffset.x += followedDeltaX;
                
                if(smoothMovement)
                    directionalOffset.x = directionalOffset.x * filteringFactor + lastOffset * (1.0 - filteringFactor);
                
                if(directionMultiplierX > 0)
                {
                    if(directionalOffset.x  < directionalOffsetToReach.x)
                    {
                        directionalOffset.x = directionalOffsetToReach.x;
                        reachingOffsetX = false;
                    }
                }
                else
                {
                    if(directionalOffset.x > directionalOffsetToReach.x)
                    {
                        directionalOffset.x = directionalOffsetToReach.x;
                        reachingOffsetX = false;
                    }
                }
            }
            
            if(direction.x/previousDirectionVector.x <= 0 || (direction.x == 0 && previousDirectionVector.x == 0))
            {
                if(direction.x >= 0){
                    directionMultiplierX = -1.0f;
                }
                else{
                    directionMultiplierX = 1.0f;
                }
                
                directionalOffsetToReach.x = -importantArea.width * winSize.width * 0.5 * directionMultiplierX;
                reachingOffsetX = true;
            }
            
            
            if(reachingOffsetY)
            {
                float lastOffset = directionalOffset.y;
                directionalOffset.y += followedDeltaY;
                
                if(smoothMovement)
                    directionalOffset.y = directionalOffset.y * filteringFactor + lastOffset * (1.0 - filteringFactor);
                
                if(directionMultiplierY > 0)
                {
                    if(directionalOffset.y  > directionalOffsetToReach.y)
                    {
                        directionalOffset.y = directionalOffsetToReach.y;
                        reachingOffsetY = false;
                    }
                }
                else
                {
                    if(directionalOffset.y < directionalOffsetToReach.y)
                    {
                        directionalOffset.y = directionalOffsetToReach.y;
                        reachingOffsetY = false;
                    }
                }
            }
            
            
            if(direction.y/previousDirectionVector.y <= 0 || (direction.y == 0 && previousDirectionVector.y == 0))
            {
                if(direction.y >= 0){
                    directionMultiplierY = 1.0f;
                }
                else{
                    directionMultiplierY = -1.0f;
                }
                
                directionalOffsetToReach.y = importantArea.height * winSize.height * 0.5 * directionMultiplierY;
                
                reachingOffsetY = true;
            }
            
            previousDirectionVector = direction;
        }
        
        
        previousFollowedPosition = curPosition;
    }
    
    
    visitActiveAnimation();
    
    if(followedNode()){
        Point pt = transformToRestrictivePosition(followedNode()->getPosition());
        setPosition(pt);
    }
    setSceneView();
}

#if COCOS2D_VERSION >= 0x00030200
void LHCamera::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
#else
void LHCamera::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
#endif
{
    this->performVisit();
    
    if(renderer)
    {
#if COCOS2D_VERSION >= 0x00030200
        Node::visit(renderer, parentTransform, parentFlags);
#else
        Node::visit(renderer, parentTransform, parentTransformUpdated);
#endif
    }
    
    wasUpdated= true;
}
