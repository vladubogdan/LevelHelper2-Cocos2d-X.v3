#ifndef __LH_SCENE_COLLISION_HANDLING_DEMO_H__
#define __LH_SCENE_COLLISION_HANDLING_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneCollisionHandlingDemo : public LHSceneDemo
{
public:

    static LHSceneCollisionHandlingDemo* create();
    
    LHSceneCollisionHandlingDemo();
    virtual ~LHSceneCollisionHandlingDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
    
#if LH_USE_BOX2D
    
    virtual bool shouldDisableContactBetweenNodes(Node* nodeA, Node* nodeB);
    virtual void didBeginContact(LHContactInfo contact);
    virtual void didEndContact(LHContactInfo contact);
    
#else//chipmunk
    
    bool onContactBegin(PhysicsContact& contact);
    
#endif
    
};

#endif // __LH_SCENE_COLLISION_HANDLING_DEMO_H__
