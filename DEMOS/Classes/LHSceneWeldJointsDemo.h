#ifndef __LH_SCENE_WELD_DEMO_H__
#define __LH_SCENE_WELD_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneWeldJointsDemo : public LHSceneDemo
{
public:

    static LHSceneWeldJointsDemo* create();
    
    LHSceneWeldJointsDemo();
    virtual ~LHSceneWeldJointsDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
private:
};

#endif // __LH_SCENE_WELD_DEMO_H__
