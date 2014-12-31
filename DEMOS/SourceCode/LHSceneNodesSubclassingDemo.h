#ifndef __LH_SCENE_NODES_SUBCLASSING_DEMO_H__
#define __LH_SCENE_NODES_SUBCLASSING_DEMO_H__

#include "cocos2d.h"
#include "LHSceneDemo.h"

class LHSceneNodesSubclassingDemo : public LHSceneDemo
{
public:

    static LHSceneNodesSubclassingDemo* create();
    
    LHSceneNodesSubclassingDemo();
    virtual ~LHSceneNodesSubclassingDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);

    virtual std::string className();
    
    virtual LevelHelperNodeTypeSubclass createNodeObjectForSubclassWithName(const std::string subclassTypeName, const std::string lhTypeName);
    
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
};

#endif // __LH_SCENE_NODES_SUBCLASSING_DEMO_H__
