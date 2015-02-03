
#ifndef __LH_SCENE_ASSET_WITH_JOINTS_DEMO_H__
#define __LH_SCENE_ASSET_WITH_JOINTS_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneAssetWithJointsDemo : public LHSceneDemo
{
public:

    static LHSceneAssetWithJointsDemo* create();
    
    LHSceneAssetWithJointsDemo();
    virtual ~LHSceneAssetWithJointsDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);

private:
    
};

#endif // __LH_SCENE_ASSET_WITH_JOINTS_DEMO_H__
