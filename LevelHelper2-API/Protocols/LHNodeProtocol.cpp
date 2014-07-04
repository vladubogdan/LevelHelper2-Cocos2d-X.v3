//
//  LHNodeProtocol.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 24/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHNodeProtocol.h"
#include "LHDictionary.h"
#include "LHArray.h"

#include "LHConfig.h"
#include "LHUserProperties.h"
#include "LHValue.h"

#include "LHScene.h"
#include "LHSprite.h"
#include "LHShape.h"
#include "LHBezier.h"
#include "LHShape.h"
#include "LHRopeJointNode.h"
#include "LHParallax.h"
#include "LHParallaxLayer.h"
#include "LHWater.h"
#include "LHNode.h"
#include "LHAsset.h"
#include "LHCamera.h"


#include "LHGameWorldNode.h"
#include "LHUINode.h"
#include "LHBackUINode.h"
#include "LHDevice.h"

LHNodeProtocol::LHNodeProtocol():name("Untitled"),userProperty(NULL)
{
}
LHNodeProtocol::~LHNodeProtocol()
{
    if(userProperty){
        delete userProperty;
        userProperty = NULL;
    }
}

void LHNodeProtocol::setTags(LHArray* _tags_)
{
    tags.clear();
    for(int i = 0; i < _tags_->count(); ++i)
    {
        tags.push_back(_tags_->stringAtIndex(i));
    }
}

std::string LHNodeProtocol::getUuid()
{
    return uuid;
}
void LHNodeProtocol::setUuid(std::string value)
{
    uuid = std::string(value);
}

Node* LHNodeProtocol::LHGetNode(LHNodeProtocol* prot)
{
    return dynamic_cast<Node*>(prot);
}



void LHNodeProtocol::loadUserPropertyWithDictionary(LHDictionary* dict)
{
    if(dict->objectForKey("userPropertyInfo"))
    {
        Node* node = LH_GET_NODE_FROM_NODE_PROTOCOL(this);
        if(!node)return;
        
        LHDictionary* userPropInfo  = dict->dictForKey("userPropertyInfo");
        std::string userPropClassName = dict->stringForKey("userPropertyName");
        
        LHUserPropertyProtocol* propInstance = LHUserCustomPropertiesManager::customClassInstanceWithName(userPropClassName, node);
        
        if(propInstance)
        {
            propInstance->setPropertiesFromDictionary(userPropInfo);
            userProperty = propInstance;
        }
    }
}

void LHNodeProtocol::loadChildrenFromDictionary(LHDictionary* dict)
{
    LHArray* childrenInfo = dict->arrayForKey("children");
    if(childrenInfo)
    {
        Node* node = dynamic_cast<Node*>(this);
        if(!node)return;

        for(int i = 0; i < childrenInfo->count(); ++i)
        {
            LHDictionary* childInfo = childrenInfo->dictAtIndex(i);
            Node* newNode = LHScene::createLHNodeWithDictionary(childInfo, node);
            #pragma unused (newNode)
        }
    }
}


Node* LHNodeProtocol::getChildNodeWithName(const std::string& name)
{
    Node* node = LH_GET_NODE_FROM_NODE_PROTOCOL(this);
    if(!node)return NULL;
    
    auto& children = node->getChildren();
    for( const auto &n : children)
    {
        LHNodeProtocol* nProt = dynamic_cast<LHNodeProtocol*>(n);
        if(nProt)
        {
            if(nProt->getName() == name)
            {
                return n;
            }
            
            Node* retNode = nProt->getChildNodeWithName(name);
            if(retNode)
            {
                return retNode;
            }
        }
    }
    return NULL;
}

Node* LHNodeProtocol::getChildNodeWithUUID(const std::string& uuid)
{
    Node* node = LH_GET_NODE_FROM_NODE_PROTOCOL(this);
    if(!node){
        return NULL;
    }
    
    auto& children = node->getChildren();
    for( const auto &n : children){
        LHNodeProtocol* nProt = dynamic_cast<LHNodeProtocol*>(n);
        if(nProt){
            if(nProt->getUuid() == uuid){
                return n;
            }
            
            Node* retNode = nProt->getChildNodeWithUUID(uuid);
            if(retNode){
                return retNode;
            }
        }
    }
    return NULL;
}

__Array* LHNodeProtocol::getChildrenWithTags(const std::vector<std::string>& tagValues, bool any)
{
    __Array* temp = __Array::create();
    
    Node* node = LH_GET_NODE_FROM_NODE_PROTOCOL(this);
    if(!node){
        return NULL;
    }
    
    auto& children = node->getChildren();
    for( const auto &n : children){
        LHNodeProtocol* nProt = dynamic_cast<LHNodeProtocol*>(n);
        if(nProt){
            std::vector<std::string> childTags = nProt->getTags();
            
            int foundCount = 0;
            bool foundAtLeastOne = false;
            
            for(size_t i = 0; i < childTags.size(); ++i)
            {
                std::string tg = childTags[i];
                
                for(size_t j = 0; j < tagValues.size();++j)
                {
                    std::string st = tagValues[j];
                    if(st == tg)
                    {
                        ++foundCount;
                        foundAtLeastOne = true;
                        if(any){
                            break;
                        }
                    }
                }
                
                if(any && foundAtLeastOne){
                    temp->addObject(n);
                }
            }
            
            if(!any && foundAtLeastOne && foundCount == tagValues.size()){
                temp->addObject(n);
            }
            
            __Array* childArray = nProt->getChildrenWithTags(tagValues, any);
            if(childArray){
                temp->addObjectsFromArray(childArray);
            }
        }
    }
    return temp;
}

Point LHNodeProtocol::positionForNode(Node* node, Point unitPos)
{
    LHScene* scene = (LHScene*)node->getScene();
    
    Size designSize = scene->getDesignResolutionSize();
    Point offset    = scene->getDesignOffset();
    
    Point designPos = Point();
    
    if(node->getParent() == nullptr ||
       node->getParent() == scene ||
       node->getParent() == scene->getGameWorldNode() ||
       node->getParent() == scene->getUINode()  ||
       node->getParent() == scene->getBackUINode())
    {
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

void LHNodeProtocol::loadGenericInfoFromDictionary(LHDictionary* dict){
    
    if(dict->objectForKey("name"))
        setName(dict->stringForKey("name"));
    
    setUuid(dict->stringForKey("uuid"));
    setTags(dict->arrayForKey("tags"));
    loadUserPropertyWithDictionary(dict);
    
    Node* node = LH_GET_NODE_FROM_NODE_PROTOCOL(this);
    if(!node)return;
    
    if(dict->objectForKey("size")){
        node->setContentSize(dict->sizeForKey("size"));
    }
    
    if(dict->objectForKey("colorOverlay")){
        node->setColor(dict->colorForKey("colorOverlay"));
    }
    
    if(dict->objectForKey("alpha")){
        node->setOpacity(dict->floatForKey("alpha"));
    }
    if(dict->objectForKey("rotation")){
        node->setRotation(dict->floatForKey("rotation"));
    }
    
    if(dict->objectForKey("zOrder")){
        node->setZOrder(dict->floatForKey("zOrder"));
    }
    
    if(dict->objectForKey("scale")){
        Point scl = dict->pointForKey("scale");
        node->setScaleX(scl.x);
        node->setScaleY(scl.y);
    }
}

void LHNodeProtocol::loadTransformationInfoFromDictionary(LHDictionary* dict)
{
    Node* node = LH_GET_NODE_FROM_NODE_PROTOCOL(this);
    if(!node)return;
    
    if(dict->objectForKey("anchor")){
        Point anchor = dict->pointForKey("anchor");
        anchor.y = 1.0f - anchor.y;
        node->setAnchorPoint(anchor);
    }

    
    if(dict->objectForKey("generalPosition"))
    {
        Point unitPos   = dict->pointForKey("generalPosition");
        Point pos       = LHNodeProtocol::positionForNode(node, unitPos);
        
        LHDictionary* devPositions = dict->dictForKey("devicePositions");
        if(devPositions)
        {
            std::string unitPosStr = LHDevice::devicePosition(devPositions, LH_SCREEN_RESOLUTION);
            
            if(unitPosStr.length()>0){
                Point unitPos = PointFromString(unitPosStr);
                pos = LHScene::positionForNode(node, unitPos);
            }
        }
        node->setPosition(pos);
    }
}


Node* LHNodeProtocol::createLHNodeWithDictionary(LHDictionary* childInfo, Node* prnt)
{
    
    std::string nodeType = childInfo->stringForKey("nodeType");
    
    LHScene* scene = NULL;
    
    if( LHScene::isLHScene(prnt)){
        scene = (LHScene*)prnt;
    }
    else if(LHScene::isLHScene(prnt->getScene())){
        scene = (LHScene*)prnt->getScene();
    }
    
    
    if(nodeType == "LHGameWorldNode")
    {
        return LHGameWorldNode::nodeWithDictionary(childInfo, prnt);
    }
    else if(nodeType == "LHBackUINode")
    {
        return LHBackUINode::nodeWithDictionary(childInfo, prnt);
    }
    else if(nodeType == "LHUINode")
    {
        return LHUINode::nodeWithDictionary(childInfo, prnt);
    }
    else if(nodeType == "LHSprite")
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
    else if(nodeType == "LHWaves")
    {
        LHWater* wt = LHWater::waterWithDictionary(childInfo, prnt);
        return wt;
    }
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

