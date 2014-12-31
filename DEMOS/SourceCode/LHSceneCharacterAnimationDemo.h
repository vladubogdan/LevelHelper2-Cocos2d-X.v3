#ifndef __LH_SCENE_CHARACTER_ANIM_DEMO_H__
#define __LH_SCENE_CHARACTER_ANIM_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneCharacterAnimationDemo : public LHSceneDemo
{
public:

    static LHSceneCharacterAnimationDemo* create();
    
    LHSceneCharacterAnimationDemo();
    virtual ~LHSceneCharacterAnimationDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();

    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
};

#endif // __LH_SCENE_CHARACTER_ANIM_DEMO_H__
