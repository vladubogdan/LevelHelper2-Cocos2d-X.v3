//
//  LHParallax.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHParallax.h"
#include "LHUtils.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHParallaxLayer.h"
#include "LHDevice.h"
#include "LHNode.h"
#include "LHGameWorldNode.h"
#include "LHCamera.h"

LHParallax::LHParallax()
{
    _followedNode = nullptr;
    lastPosition = Point();
}

LHParallax::~LHParallax()
{
    _followedNode = nullptr;
}

LHParallax* LHParallax::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHParallax *ret = new LHParallax();
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

bool LHParallax::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        prnt->addChild(this);
        this->loadGenericInfoFromDictionary(dict);
        
        this->loadTransformationInfoFromDictionary(dict);
                
        this->loadChildrenFromDictionary(dict);
        
        if(dict->objectForKey("followedNodeUUID")){
            _followedNodeUUID = dict->stringForKey("followedNodeUUID");
        }
        
        this->createAnimationsFromDictionary(dict);
        
        return true;
    }
    return false;
}

Node* LHParallax::followedNode()
{
    if(_followedNode == nullptr && _followedNodeUUID.length()> 0){
        _followedNode = ((LHScene*)getScene())->getGameWorldNode()->getChildNodeWithUUID(_followedNodeUUID);
        if(_followedNode){
            _followedNodeUUID ="";
        }
    }
    return _followedNode;
}
void LHParallax::followNode(Node* node){
    _followedNode = node;
}

#if COCOS2D_VERSION >= 0x00030200
void LHParallax::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
#else
void LHParallax::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
#endif
{
    visitActiveAnimation();
    transformLayerPositions();
   
    if(renderer)
    {
#if COCOS2D_VERSION >= 0x00030200
        Node::visit(renderer, parentTransform, parentFlags);
#else
        Node::visit(renderer, parentTransform, parentTransformUpdated);
#endif
    }
}

void LHParallax::transformLayerPositions()
{
    LHScene* scene = (LHScene*)this->getScene();
    
    LHGameWorldNode* gwNode = scene->getGameWorldNode();
    
    float oldScale = gwNode->getScale();
    gwNode->setScale(1.0f);
    
    
    Point parallaxPos = getPosition();
    Node* followed = followedNode();
    if(followed){
        
        if(LHCamera::isLHCamera(followed)){
            if(!((LHCamera*)followed)->wasUpdated)return;
        }

        Point worldPoint = followed->convertToWorldSpaceAR(Point(0,0));
        
        if(LHCamera::isLHCamera(followed)){
            
            ((LHCamera*)followed)->setZoomValue(1);
            
            Size winSize = scene->getDesignResolutionSize();
            worldPoint = Point(winSize.width*0.5, winSize.height*0.5);
        }
        
        parallaxPos = gwNode->convertToNodeSpaceAR(worldPoint);
    }
    
    if(initialPosition.equals(Point())){
        initialPosition = parallaxPos;
    }
    

    if(!lastPosition.equals(parallaxPos))
    {
        Point deltaPos(initialPosition.x - parallaxPos.x,
                       initialPosition.y - parallaxPos.y);
        
        auto& children = this->getChildren();
        for( const auto &n : children)
        {
            LHParallaxLayer* nd = dynamic_cast<LHParallaxLayer*>(n);
            if(nd)
            {
                Point initialPos = nd->getInitialPosition();

                Point pt(initialPos.x - deltaPos.x*(nd->getRatioX()),
                         initialPos.y - deltaPos.y*(nd->getRatioY()));
                
                nd->setPosition(pt);
            }
        }
    }
    lastPosition = parallaxPos;
    
    gwNode->setScale(oldScale);
    if(followed&& LHCamera::isLHCamera(followed)){
        ((LHCamera*)followed)->setZoomValue(oldScale);        
    }
}
