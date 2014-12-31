#ifndef __LH_SCENE_CAMERA_FOLLOW_DEMO_H__
#define __LH_SCENE_CAMERA_FOLLOW_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneCameraFollowDemo : public LHSceneDemo
{
public:

    static LHSceneCameraFollowDemo* create();
    
    LHSceneCameraFollowDemo();
    virtual ~LHSceneCameraFollowDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
private:
    bool didChangeX;
};

#endif // __LH_SCENE_CAMERA_FOLLOW_DEMO_H__
