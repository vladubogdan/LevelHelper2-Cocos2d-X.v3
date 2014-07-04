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

#if LH_USE_BOX2D
class b2Body;
#endif

class LHDictionary;
class LHArray;
class LHScene;

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
    
    void visitPhysicsProtocol();
    
    static Node* LHGetNode(LHPhysicsProtocol* prot);
    
    /**
     Removed the physics body from the node. The Cocos2d node will still be alive.
     If you want to remove the node call "removeFromParent" instead.
     Note that you won't be able to recreate the body after removal without recreating the entire Cocos2d node.
     If you need the physics body at a later time you may want to change the physics type to No Physics.
     */
    void removeBody();
    
    void updatePhysicsTransform();
    void updatePhysicsScale();
    
#if LH_USE_BOX2D

    AffineTransform absoluteTransform();

    /**
     Returns the Box2d body created on this sprite or NULL if sprite has no physics.
     */
    b2Body* getBox2dBody(){return _body;}

    /**
     Take the Box2d body from a common Node* object. Helper function in order to not deal with casts.
     */
    static inline b2Body* LHGetBox2dBody(Node * _node_)
    {
        LHPhysicsProtocol* prot = dynamic_cast<LHPhysicsProtocol*>(_node_);
        if(!prot)return NULL;
        return prot->getBox2dBody();
    }
    
    virtual void updatePosition(const cocos2d::Vec2 &pos) = 0;
    virtual void updateRotation(float rotation) = 0;
    
private:
    Point previousScale;
    b2Body* _body;
#endif
};

#define LH_GET_BOX2D_BODY LHPhysicsProtocol::LHGetBox2dBody
#define LH_GET_NODE_FROM_PHYSICS_PROTOCOL LHPhysicsProtocol::LHGetNode


#endif //__LEVELHELPER_API_PHYSICS_PROTOCOL_H__
