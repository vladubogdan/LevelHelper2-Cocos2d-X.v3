#ifndef __LH_SCENE_REMOVE_ON_COLLISION_H__
#define __LH_SCENE_REMOVE_ON_COLLISION_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneRemoveOnCollisionDemo : public LHSceneDemo
{
public:

    static LHSceneRemoveOnCollisionDemo* create();
    
    LHSceneRemoveOnCollisionDemo();
    virtual ~LHSceneRemoveOnCollisionDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
    void handleCandyCollisionWithNode(Node* candy, Node* other);
    bool disableCandyCollisionWithNode(Node* node);
    
#if LH_USE_BOX2D
    
    virtual bool shouldDisableContactBetweenNodes(Node* nodeA, Node* nodeB);
    virtual void didBeginContact(const LHContactInfo& contact);
    
#else//chipmunk
    
    bool onContactBegin(PhysicsContact& contact);
    
#endif
    
};

#endif // __LH_SCENE_COLLISION_HANDLING_DEMO_H__
