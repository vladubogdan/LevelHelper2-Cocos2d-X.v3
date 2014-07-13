#ifndef __LH_SCENE_USER_PROPERTIES_DEMO_H__
#define __LH_SCENE_USER_PROPERTIES_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneUserPropertiesDemo : public LHSceneDemo
{
public:

    static LHSceneUserPropertiesDemo* create();
    
    LHSceneUserPropertiesDemo();
    virtual ~LHSceneUserPropertiesDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
};

#endif // __LH_SCENE_USER_PROPERTIES_DEMO_H__
