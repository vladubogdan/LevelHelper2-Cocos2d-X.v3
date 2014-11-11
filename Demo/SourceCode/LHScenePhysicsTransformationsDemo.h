#ifndef __LH_SCENE_PHYSICS_TRANSFORMATIONS_DEMO_H__
#define __LH_SCENE_PHYSICS_TRANSFORMATIONS_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHScenePhysicsTransformationsDemo : public LHSceneDemo
{
public:

    static LHScenePhysicsTransformationsDemo* create();
    
    LHScenePhysicsTransformationsDemo();
    virtual ~LHScenePhysicsTransformationsDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    
private:

};

#endif // __LH_SCENE_PHYSICS_TRANSFORMATIONS_DEMO_H__
