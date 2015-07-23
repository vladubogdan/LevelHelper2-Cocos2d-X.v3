#ifndef __LH_SCENE_JOINTWITHCAMERA_BUGFIX_H__
#define __LH_SCENE_JOINTWITHCAMERA_BUGFIX_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneJointWithCameraBUGFix : public LHSceneDemo
{
public:

    static LHSceneJointWithCameraBUGFix* create();
    
    LHSceneJointWithCameraBUGFix();
    virtual ~LHSceneJointWithCameraBUGFix();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
};

#endif // __LH_SCENE_JOINTWITHCAMERA_BUGFIX_H__
