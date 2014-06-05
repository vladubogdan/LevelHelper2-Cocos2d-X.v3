#ifndef __LH_SCENE_SUBCLASS_CUT_ROPE_H__
#define __LH_SCENE_SUBCLASS_CUT_ROPE_H__

#include "cocos2d.h"
#include "LevelHelper2API.h"

class LHSceneCutRope : public LHScene
{
public:
    static LHSceneCutRope* createWithContentOfFile(const std::string& plistLevelFile);
    
    LHSceneCutRope();
    virtual ~LHSceneCutRope();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);
    
    
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch *touch, Event *event);
};

#endif // __LH_SCENE_SUBCLASS_CUT_ROPE_H__
