#ifndef __LH_SCENE_SUBCLASS_H__
#define __LH_SCENE_SUBCLASS_H__

#include "cocos2d.h"
#include "LevelHelper2API.h"

class LHSceneSubclass : public LHScene
{
public:
    static cocos2d::Scene* createScene();

    LHSceneSubclass();
    virtual ~LHSceneSubclass();
    
    bool initWithContentOfFile(const std::string& plistLevelFile);
    
};

#endif // __LH_SCENE_SUBCLASS_H__
