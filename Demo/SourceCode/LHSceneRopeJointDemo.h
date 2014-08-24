#ifndef __LH_SCENE_ROPE_JOINT_DEMO_H__
#define __LH_SCENE_ROPE_JOINT_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneRopeJointDemo : public LHSceneDemo
{
public:

    static LHSceneRopeJointDemo* create();
    
    LHSceneRopeJointDemo();
    virtual ~LHSceneRopeJointDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
    virtual void didCutRopeJoint(LHRopeJointNode* jt);

};

#endif // __LH_SCENE_ROPE_JOINT_DEMO_H__
