#ifndef __LH_SCENE_INTRODUCTION_DEMO_H__
#define __LH_SCENE_INTRODUCTION_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneIntroductionDemo : public LHSceneDemo
{
public:

    static LHSceneIntroductionDemo* create();
    
    LHSceneIntroductionDemo();
    virtual ~LHSceneIntroductionDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
};

#endif // __LH_SCENE_INTRODUCTION_DEMO_H__
