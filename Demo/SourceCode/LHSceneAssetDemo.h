#ifndef __LH_SCENE_ASSET_DEMO_H__
#define __LH_SCENE_ASSET_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneAssetDemo : public LHSceneDemo
{
public:

    static LHSceneAssetDemo* create();
    
    LHSceneAssetDemo();
    virtual ~LHSceneAssetDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    
private:
};

#endif // __LH_SCENE_ASSET_DEMO_H__
