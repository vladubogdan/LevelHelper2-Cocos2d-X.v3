
#ifndef __LH_SCENE_SKELETAL_CHARACTER_DEMO_H__
#define __LH_SCENE_SKELETAL_CHARACTER_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneSkeletalCharacterDemo : public LHSceneDemo
{
public:

    static LHSceneSkeletalCharacterDemo* create();
    
    LHSceneSkeletalCharacterDemo();
    virtual ~LHSceneSkeletalCharacterDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
private:
    
};

#endif // __LH_SCENE_SKELETAL_CHARACTER_DEMO_H__
