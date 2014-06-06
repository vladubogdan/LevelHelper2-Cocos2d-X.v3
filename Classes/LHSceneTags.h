#ifndef __LH_SCENE_SUBCLASS_TAGS_H__
#define __LH_SCENE_SUBCLASS_TAGS_H__

#include "cocos2d.h"
#include "LevelHelper2API.h"

class LHSceneTags : public LHScene
{
public:
    static LHSceneTags* createWithContentOfFile(const std::string& plistLevelFile);
    
    LHSceneTags();
    virtual ~LHSceneTags();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);
};

#endif // __LH_SCENE_SUBCLASS_TAGS_H__
