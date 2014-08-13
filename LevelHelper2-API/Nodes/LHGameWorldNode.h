//
//  LHGameWorldNode.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_GAME_WORLD_NODE_H__
#define __LEVELHELPER_API_GAME_WORLD_NODE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHConfig.h"


#if LH_USE_BOX2D
class b2World;
class LHBox2dDebugDrawNode;
#endif

using namespace cocos2d;

/**
 LHGameWorldNode class is used to load the game world node object from a level file.
 Users can retrieve node objects by calling the scene (LHScene) getChildNodeWithName("name") method.
 */

class LHScheduledContactInfo
{
public:
    
    LHScheduledContactInfo(Node* a, Node* b, Point ctPt, float imp)
    {
        _nodeA = a;
        _nodeB = b;
        _contactPoint = ctPt;
        _impulse = imp;
    }
    virtual ~LHScheduledContactInfo(){_nodeA = nullptr; _nodeB = nullptr;}
    
    Node* getNodeA(){return _nodeA;}
    void setNodeA(Node* a){_nodeA = a;}
    
    Node* getNodeB(){return _nodeB;}
    void setNodeB(Node* b){_nodeB = b;}
    
    Point getContactPoint(){return _contactPoint;}
    float getImpulse(){return _impulse;}
    
private:
    Node* _nodeA;
    Node* _nodeB;
    Point _contactPoint;
    float _impulse;
};


class LHDictionary;
class LHScene;

class LHGameWorldNode : public Node, public LHNodeProtocol
{
public:
 
    static LHGameWorldNode* nodeWithDictionary(LHDictionary* dict, Node* prnt);

    LHGameWorldNode();
    virtual ~LHGameWorldNode();
    bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    static  bool isLHGameWorldNode(Node* obj){return (0 != dynamic_cast<LHGameWorldNode*>(obj));}
    virtual bool isGameWorldNode(){return true;}
    
#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif
    
    void update(float delta);
    
#if LH_USE_BOX2D
    
    void setBox2dFixedTimeStep(float val){ FIXED_TIMESTEP = val;}
    void setBox2dMinimumTimeStep(float val){ MINIMUM_TIMESTEP = val;}
    void setBox2dVelocityIterations(int val){ VELOCITY_ITERATIONS = val;}
    void setBox2dPositionIterations(int val){ POSITION_ITERATIONS = val;}
    void setBox2dMaxSteps(int val){ MAXIMUM_NUMBER_OF_STEPS = val;}

    
    b2World* getBox2dWorld();
    LHBox2dDebugDrawNode* _debugNode;
#endif
    
    Point getGravity();
    void setGravity(Point val);
    
private:

#if LH_USE_BOX2D
    
    friend class LHBox2dCollisionHandling;

    
    void scheduleDidBeginContactBetweenNodeA(Node* nodeA, Node* nodeB, Point contactPoint, float impulse);
    void scheduleDidEndContactBetweenNodeA(Node* nodeA, Node* nodeB);
    

    void step(float dt);
    void afterStep(float dt);
    
    b2World* _box2dWorld;
    
    float FIXED_TIMESTEP;
    float MINIMUM_TIMESTEP;
    int VELOCITY_ITERATIONS;
    int POSITION_ITERATIONS;
    int MAXIMUM_NUMBER_OF_STEPS;
    
    
    std::vector<LHScheduledContactInfo> _scheduledBeginContact;
    std::vector<LHScheduledContactInfo> _scheduledEndContact;
    
#endif

    
};

#endif //__LEVELHELPER_API_GAME_WORLD_NODE_H__
