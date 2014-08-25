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
    Point parallaxPos = getPosition();
    Node* followed = followedNode();
    if(followed){
        parallaxPos = followed->getPosition();
        
        Point anchor = followed->getAnchorPoint();
        Size content = followed->getContentSize();
        
        parallaxPos.x -= content.width*(anchor.x -0.5);
        parallaxPos.y -= content.height*(anchor.y -0.5);

        Size winSize = ((LHScene*)getScene())->getContentSize();
        
        parallaxPos.x = parallaxPos.x - winSize.width*0.5;
        parallaxPos.y = parallaxPos.y - winSize.height*0.5;
    }
    
    if(lastPosition.equals(Point())){
        lastPosition = parallaxPos;
    }
    
    if(!lastPosition.equals(parallaxPos))
    {
        Point deltaPos(parallaxPos.x - lastPosition.x,
                       parallaxPos.y - lastPosition.y);

        auto& children = this->getChildren();
        for( const auto &n : children)
        {
            LHParallaxLayer* nd = dynamic_cast<LHParallaxLayer*>(n);
            if(nd)
            {
                Point curPos = nd->getPosition();
                
                Point pt(curPos.x + deltaPos.x*(-nd->getRatioX()),
                         curPos.y + deltaPos.y*(-nd->getRatioY()));
                
                nd->setPosition(pt);
            }
        }
    }
    lastPosition = parallaxPos;
}
