//
//  LHJointNodeProtocol.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 24/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_JOINT_NODE_PROTOCOL_H__
#define __LEVELHELPER_API_JOINT_NODE_PROTOCOL_H__

#include "cocos2d.h"

#if LH_USE_BOX2D
class b2Joint;
#endif

class LHDictionary;
class LHArray;
class LHScene;

using namespace cocos2d;


/**
LevelHelper-2 joint nodes conforms to this protocol.
 */

class LHJointNodeProtocol
{
public:

    LHJointNodeProtocol();
    virtual ~LHJointNodeProtocol();
    
    
    /**
     Returns the point where the joint is connected with the first body. In scene coordinates.
     */
    Point getAnchorA();
    
    /**
     Returns the point where the joint is connected with the second body. In scene coordinates.
     */
    Point getAnchorB();
    
    /**
     Returns the actual Box2d or Cocos2d-X(chipmunk) joint object.
     */
    
#if LH_USE_BOX2D

    b2Joint* getJoint();

#else//chipmunk

    PhysicsJoint* getJoint();

#endif//LH_USE_BOX2D
    
    void removeJoint();
    
    void findConnectedNodes();
    void loadJointInfoFromDictionary(LHDictionary* dict);
    
    Node* getNodeA(){return _nodeA;}
    Node* getNodeB(){return _nodeB;}
    
    Point getLocalAnchorA();
    Point getLocalAnchorB();
    
    
    bool getCollideConnected();
    
#if LH_USE_BOX2D
    
    void setJoint(b2Joint* jt);
    
#else//chipmunk
    
    void setJoint(PhysicsJoint* jt);
    
#endif//LH_USE_BOX2D

private:
    
    
#if LH_USE_BOX2D
    b2Joint* _joint;
#else
    PhysicsJoint* _joint;
#endif
    
    std::string _nodeAUUID;
    std::string _nodeBUUID;
    
    Point       _relativePosA;
    Point       _relativePosB;

    Node*       _nodeA;
    Node*       _nodeB;
    
    bool        _collideConnected;

};

#endif //__LEVELHELPER_API_JOINT_NODE_PROTOCOL_H__
