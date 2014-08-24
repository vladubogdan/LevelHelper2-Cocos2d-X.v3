#ifndef __LH_SCENE_PULLEY_JOINT_DEMO_H__
#define __LH_SCENE_PULLEY_JOINT_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"
#include "LHConfig.h"

#if LH_USE_BOX2D
#include "Box2d/Box2d.h"
#endif

class LHScenePulleyJointDemo : public LHSceneDemo
{
public:

    static LHScenePulleyJointDemo* create();
    
    LHScenePulleyJointDemo();
    virtual ~LHScenePulleyJointDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch *touch, Event *event);

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

#endif // __LH_SCENE_PULLEY_JOINT_DEMO_H__
