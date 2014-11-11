//
//  CameraPhysicsTest.h
//
//
//  Created by Jeffrey Owen on 11/09/2014.
//
//

#ifndef __CameraPhysicsTest__
#define __CameraPhysicsTest__

#include <iostream>
#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "LHSceneDemo.h"

class CameraPhysicsTest : public LHSceneDemo
{
public:
    static CameraPhysicsTest* create();
    CameraPhysicsTest();
    virtual ~CameraPhysicsTest();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);
    
    virtual std::string className();
    
    
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
    virtual void onTouchesCancelled(const std::vector<Touch*>& touches, Event *event);
    
    void createMouseJointForTouchLocation(Point point);
    void setTargetOnMouseJoint(Point point);
    void destroyMouseJoint();
#if LH_USE_BOX2D
    b2MouseJoint* mouseJoint;
#endif
};

#endif /* defined(CameraPhysicsTest) */
