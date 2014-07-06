#ifndef __LH_SCENE_PARALLAX_DEMO_H__
#define __LH_SCENE_PARALLAX_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneParallaxDemo : public LHSceneDemo
{
public:

    static LHSceneParallaxDemo* create();
    
    LHSceneParallaxDemo();
    virtual ~LHSceneParallaxDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
};

#endif // __LH_SCENE_PARALLAX_DEMO_H__
