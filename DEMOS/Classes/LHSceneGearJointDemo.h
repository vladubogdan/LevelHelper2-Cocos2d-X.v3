#ifndef __LH_SCENE_GEAR_JOINT_DEMO_H__
#define __LH_SCENE_GEAR_JOINT_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"
#include "LHConfig.h"

#if LH_USE_BOX2D
#include "Box2d/Box2d.h"
#endif

class LHSceneGearJointDemo : public LHSceneDemo
{
public:

    static LHSceneGearJointDemo* create();
    
    LHSceneGearJointDemo();
    virtual ~LHSceneGearJointDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesCancelled(const std::vector<Touch*>& touches, Event* event);

private:

#if LH_USE_BOX2D
    b2MouseJoint* mouseJoint;
    
#else//chipmunk
    
    Node* touchedNode;
    bool touchedNodeWasDynamic;
    
#endif
    
    void createMouseJointForTouchLocation(Point point);
    void setTargetOnMouseJoint(Point point);
    void destroyMouseJoint();
};

#endif // __LH_SCENE_GEAR_JOINT_DEMO_H__
