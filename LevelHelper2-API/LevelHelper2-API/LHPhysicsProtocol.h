//
//  LHPhysicsProtocol.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 24/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_PHYSICS_PROTOCOL_H__
#define __LEVELHELPER_API_PHYSICS_PROTOCOL_H__

#include "cocos2d.h"
#include "LHConfig.h"

#if LH_USE_BOX2D
class b2Body;
#endif

class LHDictionary;
class LHArray;
class LHScene;
class LHAsset;

using namespace cocos2d;


/**
 LevelHelper-2 nodes that can have physics conforms to this protocol.
 */

class LHPhysicsProtocol
{
public:

    LHPhysicsProtocol();
    virtual ~LHPhysicsProtocol();

    void loadPhysicsFromDictionary(LHDictionary* dict, LHScene* scene);
    void loadPhysicsFromValueMap(ValueMap value_Map, LHScene* scene);
    
    void visitPhysicsProtocol();
    
    static Node* LHGetNode(LHPhysicsProtocol* prot);
    
    /**
     Removed the physics body from the node. The Cocos2d-X node will still be alive.
     If you want to remove the node call "removeFromParent" instead.
     Note that you won't be able to recreate the body after removal without recreating the entire Cocos2d node.
     If you need the physics body at a later time you may want to change the physics type to No Physics.
     */
    void removeBody();
    
    void updatePhysicsTransform();
    void updatePhysicsScale();
    
    void shouldRemoveBody();
    
#if LH_USE_BOX2D

    __Array* jointList();
    bool removeAllAttachedJoints();
    
    AffineTransform absoluteTransform();

    /**
     Returns the Box2d body created on this sprite or NULL if sprite has no physics.
     */
    b2Body* getBox2dBody(){ return _body;}

    /**
     Set the Box2d body that was manually created by the user. Use "removeBody" to delete the body. 
     If you manually remove the body somewhere in your code make sure you call this method with a NULL argument. e.g node->setBox2dBody(NULL);
     */
    void setBox2dBody(b2Body* val){_body = val;}
    
    /**
     Take the Box2d body from a common Node* object. Helper function in order to not deal with casts.
     */
    static inline b2Body* LHGetBox2dBody(Node * _node_)
    {
        LHPhysicsProtocol* prot = dynamic_cast<LHPhysicsProtocol*>(_node_);
        if(!prot)return NULL;
        return prot->getBox2dBody();
    }
    
    //following method are mandatory to be overwritten when using box2d in order to compute the transformations
    virtual void setPosition(const cocos2d::Vec2 &pos) = 0;
    virtual void setRotation(float rotation) = 0;
    virtual void setScaleX(float scaleX) = 0;
    virtual void setScaleY(float scaleY) = 0;
    virtual void setScale(float scaleX, float scaleY) = 0;
    virtual void updatePosition(const cocos2d::Vec2 &pos) = 0;
    virtual void updateRotation(float rotation) = 0;
    
private:
    
    bool scheduledForRemoval;
    Point previousScale;
    b2Body* _body;
    __Array* subShapes;
#endif
    
    LHAsset* assetParent();
};

#define LH_GET_BOX2D_BODY LHPhysicsProtocol::LHGetBox2dBody
#define LH_GET_NODE_FROM_PHYSICS_PROTOCOL LHPhysicsProtocol::LHGetNode


#endif //__LEVELHELPER_API_PHYSICS_PROTOCOL_H__
