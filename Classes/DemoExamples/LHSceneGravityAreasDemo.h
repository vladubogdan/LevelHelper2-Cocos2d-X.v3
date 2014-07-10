#ifndef __LH_SCENE_GRAVITYAREAS_DEMO_H__
#define __LH_SCENE_GRAVITYAREAS_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneGravityAreasDemo : public LHSceneDemo
{
public:

    static LHSceneGravityAreasDemo* create();
    
    LHSceneGravityAreasDemo();
    virtual ~LHSceneGravityAreasDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
};

#endif // __LH_SCENE_GRAVITYAREAS_DEMO_H__
