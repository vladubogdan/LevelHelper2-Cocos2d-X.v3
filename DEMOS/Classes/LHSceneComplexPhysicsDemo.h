#ifndef __LH_SCENE_COMPLEX_PHYSICS_DEMO_H__
#define __LH_SCENE_COMPLEX_PHYSICS_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneComplexPhysicsDemo : public LHSceneDemo
{
public:

    static LHSceneComplexPhysicsDemo* create();
    
    LHSceneComplexPhysicsDemo();
    virtual ~LHSceneComplexPhysicsDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
    virtual void didBeginContact(LHContactInfo contact);
    virtual void didEndContact(LHContactInfo contact);
    
};

#endif // __LH_SCENE_COMPLEX_PHYSICS_DEMO_H__
