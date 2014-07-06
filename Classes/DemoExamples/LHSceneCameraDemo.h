#ifndef __LH_SCENE_CAMERA_DEMO_H__
#define __LH_SCENE_CAMERA_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneCameraDemo : public LHSceneDemo
{
public:

    static LHSceneCameraDemo* create();
    
    LHSceneCameraDemo();
    virtual ~LHSceneCameraDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
};

#endif // __LH_SCENE_CAMERA_DEMO_H__
