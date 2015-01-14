#ifndef __LH_SCENE_DEMO_H__
#define __LH_SCENE_DEMO_H__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "UIButton.h"
#include "UIWidget.h"
#include <vector>
#include <typeinfo>

#include "extensions/cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::ui;


class LHSceneDemo : public LHScene
{
public:
    
    static LHSceneDemo* create();
    
    LHSceneDemo();
    virtual ~LHSceneDemo();
    virtual bool initWithContentOfFile(const std::string& plistLevelFile);
    
    void previousDemo(Ref *pSender, Widget::TouchEventType type);
    void restartDemo(Ref *pSender, Widget::TouchEventType type);
    void nextDemo(Ref *pSender, Widget::TouchEventType type);
   
    virtual std::string className() = 0;
private:
    
    typedef LHSceneDemo* (*createFuncPtrType)(void);
    
    std::vector<std::string> sceneNames;
    std::vector<createFuncPtrType> sceneCreators;
  
   
};

#endif // __LH_SCENE_DEMO_H__
