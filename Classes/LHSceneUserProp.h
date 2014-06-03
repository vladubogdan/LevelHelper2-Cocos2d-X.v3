#ifndef __LH_SCENE_SUBCLASS_USER_PROP_H__
#define __LH_SCENE_SUBCLASS_USER_PROP_H__

#include "cocos2d.h"
#include "LevelHelper2API.h"

class LHSceneUserProp : public LHScene
{
public:
    static LHSceneUserProp* createWithContentOfFile(const std::string& plistLevelFile);
    
    LHSceneUserProp();
    virtual ~LHSceneUserProp();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);
};

#endif // __LH_SCENE_SUBCLASS_USER_PROP_H__
