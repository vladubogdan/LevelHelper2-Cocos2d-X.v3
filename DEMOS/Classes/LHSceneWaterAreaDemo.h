#ifndef __LH_SCENE_WATER_AREA_DEMO_H__
#define __LH_SCENE_WATER_AREA_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

#if LH_USE_BOX2D
#include "Box2d/Box2d.h"
#endif

class LHSceneWaterAreaDemo : public LHSceneDemo
{
public:

    static LHSceneWaterAreaDemo* create();
    
    LHSceneWaterAreaDemo();
    virtual ~LHSceneWaterAreaDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();

    
#if LH_USE_BOX2D
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
    void onTouchesCancelled(const std::vector<Touch*>& touches, Event *event);
    void createMouseJointForTouchLocation(Point point);
    void setTargetOnMouseJoint(Point point);
    void destroyMouseJoint();
    
    b2MouseJoint* mouseJoint;

#endif
};

#endif // __LH_SCENE_WATER_AREA_DEMO_H__
