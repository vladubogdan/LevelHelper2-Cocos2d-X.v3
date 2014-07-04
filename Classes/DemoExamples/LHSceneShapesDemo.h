#ifndef __LH_SCENE_SHAPES_DEMO_H__
#define __LH_SCENE_SHAPES_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"
#include "LHConfig.h"

class LHSceneShapesDemo : public LHSceneDemo
{
public:

    static LHSceneShapesDemo* create();
    
    LHSceneShapesDemo();
    virtual ~LHSceneShapesDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch *touch, Event *event);

private:

#if LH_USE_BOX2D
    b2MouseJoint* mouseJoint;
    
#else//spritekit
    Node* touchedNode;
    bool touchedNodeWasDynamic;
    
#endif
    
    void createMouseJointForTouchLocation(Point point);
    void setTargetOnMouseJoint(Point point);
    void destroyMouseJoint();
};

#endif // __LH_SCENE_SHAPES_DEMO_H__
