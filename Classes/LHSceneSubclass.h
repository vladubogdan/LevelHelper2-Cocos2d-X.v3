#ifndef __LH_SCENE_SUBCLASS_H__
#define __LH_SCENE_SUBCLASS_H__

#include "cocos2d.h"
#include "LevelHelper2API.h"

class LHSceneSubclass : public LHScene
{
public:
    static LHSceneSubclass* createWithContentOfFile(const std::string& plistLevelFile);
    
    LHSceneSubclass();
    virtual ~LHSceneSubclass();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);
};

#endif // __LH_SCENE_SUBCLASS_H__
