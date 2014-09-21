//
//  CameraPhysicsTest.h
//
//
//  Created by Jeffrey Owen on 11/09/2014.
//
//

#ifndef __DynamicObjectsTransformAnimationTest__
#define __DynamicObjectsTransformAnimationTest__

#include <iostream>
#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "LHSceneDemo.h"

class DynamicObjectsTransformAnimationTest : public LHSceneDemo
{
public:
    static DynamicObjectsTransformAnimationTest* create();
    DynamicObjectsTransformAnimationTest();
    virtual ~DynamicObjectsTransformAnimationTest();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);
    
    virtual std::string className();
};

#endif /* defined(__DynamicObjectsTransformAnimationTest__) */
