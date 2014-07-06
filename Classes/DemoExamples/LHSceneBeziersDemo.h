#ifndef __LH_SCENE_BEZIERS_DEMO_H__
#define __LH_SCENE_BEZIERS_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneBeziersDemo : public LHSceneDemo
{
public:

    static LHSceneBeziersDemo* create();
    
    LHSceneBeziersDemo();
    virtual ~LHSceneBeziersDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
};

#endif // __LH_SCENE_BEZIERS_DEMO_H__
