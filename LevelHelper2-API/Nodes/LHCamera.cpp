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

LHCamera::LHCamera()
{
    _followedNode = nullptr;
}

LHCamera::~LHCamera()
{
    _followedNode = nullptr;
}

LHCamera* LHCamera::cameraWithDictionary(LHDictionary* dict, Node* prnt)
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
        _physicsBody = NULL;
        loadGenericInfoFromDictionary(dict);
        prnt->addChild(this);
        
        Point unitPos   = dict->pointForKey("generalPosition");
        Point pos       = LHScene::positionForNode(this, unitPos);
        
        LHDictionary* devPositions = dict->dictForKey("devicePositions");
        if(devPositions)
        {
            std::string unitPosStr = LHDevice::devicePosition(devPositions, LH_SCREEN_RESOLUTION);
            
            if(unitPosStr.length()>0){
                Point unitPos = PointFromString(unitPosStr);
                pos = LHScene::positionForNode(this, unitPos);
            }
        }
        this->setPosition(pos);

        if(dict->objectForKey("followedNodeUUID")){
            _followedNodeUUID = dict->stringForKey("followedNodeUUID");
        }
        
        _active     = dict->boolForKey("activeCamera");
        _restricted = dict->boolForKey("restrictToGameWorld");

        
        createAnimationsFromDictionary(dict);
        
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
        _followedNode = ((LHScene*)getScene())->getGameWorld()->getChildNodeWithUUID(_followedNodeUUID);
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
    Node::setPosition(transformToRestrictivePosition(position));
}

void LHCamera::setSceneView(){
    if(_active)
    {
        Point transPoint = transformToRestrictivePosition(getPosition());
        ((LHScene*)getScene())->getGameWorld()->setPosition(transPoint);
        //((LHScene*)getScene())->setPosition(transPoint);
    }
}

Point LHCamera::transformToRestrictivePosition(Point position)
{
    Node* followed = followedNode();
    if(followed){
        position = followed->getPosition();

        Point anchor = followed->getAnchorPoint();
        Size content = followed->getContentSize();
        
        position.x -= content.width*(anchor.x -0.5);
        position.y -= content.height*(anchor.y -0.5);
    }

    Size winSize = ((LHScene*)getScene())->getContentSize();
    Rect worldRect = ((LHScene*)getScene())->getGameWorldRect();

    float x = position.x;
    float y = position.y;
    
    if(!worldRect.equals(Rect()) && restrictedToGameWorld()){
        
        if(x > (worldRect.origin.x + worldRect.size.width)*0.5){
            x = MIN(x, worldRect.origin.x + worldRect.size.width - winSize.width *0.5);
        }
        else{
            x = MAX(x, worldRect.origin.x + winSize.width *0.5);
        }
        
        y = MAX(y, worldRect.origin.y + worldRect.size.height + winSize.height*0.5);
        y = MIN(y, worldRect.origin.y - winSize.height*0.5);
    }
    
    Point pt(winSize.width*0.5-x,
             winSize.height*0.5-y);
    
    return pt;
}

void LHCamera::visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated)
{
    if(!isActive())return;

    visitActiveAnimation();
    
    if(followedNode()){
        Point pt = transformToRestrictivePosition(followedNode()->getPosition());
        setPosition(pt);
    }
    setSceneView();
    Node::visit(renderer, parentTransform, parentTransformUpdated);
}
