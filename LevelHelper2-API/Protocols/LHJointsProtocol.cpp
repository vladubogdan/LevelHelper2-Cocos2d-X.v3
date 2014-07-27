//
//  LHJointsProtocol.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 24/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHJointsProtocol.h"

#include "LHDictionary.h"
#include "LHArray.h"
#include "LHConfig.h"
#include "LHValue.h"

#include "LHScene.h"

#include "LHGameWorldNode.h"
#include "LHUINode.h"
#include "LHBackUINode.h"

#if LH_USE_BOX2D
#include "Box2d/Box2d.h"
#endif

LHJointsProtocol::LHJointsProtocol()
{
    _joint = nullptr;
    _nodeA = nullptr;
    _nodeB = nullptr;
}

LHJointsProtocol::~LHJointsProtocol()
{

}

Point LHJointsProtocol::getAnchorA(){
    Point pt = _nodeA->convertToWorldSpaceAR(this->getLocalAnchorA());
    return _nodeA->getParent()->convertToNodeSpaceAR(pt);
    
}

Point LHJointsProtocol::getAnchorB(){
    Point pt = _nodeB->convertToWorldSpaceAR(this->getLocalAnchorB());
    return _nodeB->getParent()->convertToNodeSpaceAR(pt);
}

void LHJointsProtocol::findConnectedNodes()
{
    if(_nodeAUUID.length() == 0 || _nodeBUUID.length() == 0)
        return;
    
    
    Node* node = dynamic_cast<Node*>(this);
    if(!node)return;
    
    LHScene* scene = (LHScene*)node->getScene();
    
    LHNodeProtocol* parentProtocol = dynamic_cast<LHNodeProtocol*>(node->getParent());
    if(parentProtocol)
    {
        _nodeA = parentProtocol->getChildNodeWithUUID(_nodeAUUID);
        _nodeB = parentProtocol->getChildNodeWithUUID(_nodeBUUID);
    }
    else{
        _nodeA = scene->getChildNodeWithUUID(_nodeAUUID);
        _nodeB = scene->getChildNodeWithUUID(_nodeBUUID);
    }
    
    
}

void LHJointsProtocol::loadJointInfoFromDictionary(LHDictionary* dict)
{
    _joint = nullptr;
    _nodeA = nullptr;
    _nodeB = nullptr;
    
    if(dict->objectForKey("relativePosA"))//certain joints do not have an anchor (e.g. gear joint)
        _relativePosA = dict->pointForKey("relativePosA");
    
    if(dict->objectForKey("relativePosB"))//certain joints do not have a second anchor
        _relativePosB = dict->pointForKey("relativePosB");
    
    if(dict->objectForKey("spriteAUUID")){//maybe its a dummy joint
        _nodeAUUID = dict->stringForKey("spriteAUUID");
    }
    else{
        CCLOG("WARNING: Joint %s is not connected to a node", dict->stringForKey("name").c_str());
    }
    
    if(dict->objectForKey("spriteBUUID")){//maybe its a dummy joint
        _nodeBUUID = dict->stringForKey("spriteBUUID");
    }
    else{
        CCLOG("WARNING: Joint %s is not connected to a node", dict->stringForKey("name").c_str());
    }
    
    _collideConnected = dict->boolForKey("collideConnected");
}

bool LHJointsProtocol::getCollideConnected(){
    return _collideConnected;
}

Point LHJointsProtocol::getLocalAnchorA()
{
    return Point(_relativePosA.x,
                 -_relativePosA.y);
}

Point LHJointsProtocol::getLocalAnchorB()
{
    return Point( _relativePosB.x,
                 -_relativePosB.y);
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#if LH_USE_BOX2D

void LHJointsProtocol::setJoint(b2Joint* jt){
    _joint = jt;
}

b2Joint* LHJointsProtocol::getJoint(){
    return _joint;
}

void LHJointsProtocol::removeJoint()
{
    Node* node = dynamic_cast<Node*>(this);
    if(!node)return;
    
    LHScene* scene = (LHScene*)node->getScene();
    
    if(scene)
    {
        //if we dont have the scene it means the scene was changed so the box2d world will be deleted, deleting the joints also - safe
        //if we do have the scene it means the node was deleted so we need to delete the joint manually
        if(_joint){
            LHGameWorldNode* pNode = scene->getGameWorldNode();
            //if we dont have the scene it means
            b2World* world = pNode->getBox2dWorld();
            if(world){
                world->DestroyJoint(_joint);
                _joint = NULL;
            }
        }
    }
}


#else//chipmunk


void LHJointsProtocol::setJoint(PhysicsJoint* jt){
    _joint = jt;
}

PhysicsJoint* LHJointsProtocol::getJoint(){
    return _joint;
}

void LHJointsProtocol::removeJoint()
{
    if(_joint){
        _joint->removeFormWorld();
        _joint = nullptr;
    }
}

#endif//LH_USE_BOX2D



