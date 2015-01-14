//
//  LHBox2dCollisionHandling.h
//  LevelHelper2-Cocos2d-Xv3
//
//  Created by Bogdan Vladu on 06/07/14.
//  Copyright (c) 2014 VLADU BOGDAN DANIEL PFA. All rights reserved.
//

#ifndef __LEVELHELPER_API_COLLISION_HANDLING_H__
#define __LEVELHELPER_API_COLLISION_HANDLING_H__

#include "LHConfig.h"

#if LH_USE_BOX2D

#include "cocos2d.h"
#include "LHContactInfo.h"
using namespace cocos2d;

class LHScene;
class LHContactListenerPimpl;
class b2Fixture;
class b2Contact;
class b2Body;
class b2Manifold;
class b2ContactImpulse;

class LHBox2dCollisionHandling
{
public:
    
    LHBox2dCollisionHandling(LHScene* scene);
    virtual ~LHBox2dCollisionHandling();
    
    
    void preSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void postSolve(b2Contact* contact, const b2ContactImpulse* contactImpulse);
    void beginContact(b2Contact* contact);
    void endContact(b2Contact* contact);
    
    LHScene* getScene(){return _scene;}
private:
    LHScene* _scene;
    LHContactListenerPimpl* _b2Listener;
    
    LHContactInfo getContactInfoForb2Contact(b2Contact* contact);
    
    b2Fixture* getFixtureAFromContact(b2Contact* contact);
    b2Fixture* getFixtureBFromContact(b2Contact* contact);
    b2Body* getBodyAFromContact(b2Contact* contact);
    b2Body* getBodyBFromContact(b2Contact* contact);
    
    Node* getNodeAFromContact(b2Contact* contact);
    Node* getNodeBFromContact(b2Contact* contact);
    
    Point getPointFromContact(b2Contact* contact);
};

#endif

#endif //__LEVELHELPER_API_COLLISION_HANDLING_H__