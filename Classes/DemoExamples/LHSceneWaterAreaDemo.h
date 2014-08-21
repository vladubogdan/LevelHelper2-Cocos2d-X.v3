#ifndef __LH_SCENE_WATER_AREA_DEMO_H__
#define __LH_SCENE_WATER_AREA_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneWaterAreaDemo : public LHSceneDemo
{
public:

    static LHSceneWaterAreaDemo* create();
    
    LHSceneWaterAreaDemo();
    virtual ~LHSceneWaterAreaDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
};

#endif // __LH_SCENE_WATER_AREA_DEMO_H__
