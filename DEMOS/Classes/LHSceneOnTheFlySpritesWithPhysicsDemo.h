
#ifndef __LH_SCENE_ON_THE_FLY_SPRITES_WITH_PHYSICS_DEMO_H__
#define __LH_SCENE_ON_THE_FLY_SPRITES_WITH_PHYSICS_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneOnTheFlySpritesWithPhysicsDemo : public LHSceneDemo
{
public:

    static LHSceneOnTheFlySpritesWithPhysicsDemo* create();
    
    LHSceneOnTheFlySpritesWithPhysicsDemo();
    virtual ~LHSceneOnTheFlySpritesWithPhysicsDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);

private:
    
};

#endif // __LH_SCENE_ON_THE_FLY_SPRITES_WITH_PHYSICS_DEMO_H__
