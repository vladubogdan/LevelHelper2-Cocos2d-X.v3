
#include "LHSceneCutRope.h"

USING_NS_CC;

LHSceneCutRope* LHSceneCutRope::createWithContentOfFile(const std::string& plistLevelFile)
{
    LHSceneCutRope *ret = new LHSceneCutRope();
    if (ret && ret->initWithContentOfFile(plistLevelFile))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}
LHSceneCutRope::LHSceneCutRope()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneCutRope::~LHSceneCutRope()
{
    //nothing to release
}

bool LHSceneCutRope::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHScene::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto label = LabelTTF::create("Cut the rope by dragging a line", "Arial", 24);
    label->setColor(Color3B::BLACK);
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);

    
    
    return retValue;
}


bool LHSceneCutRope::onTouchBegan(Touch* touch, Event* event)
{
    //do you're own touch handling here - don't forget to call super
    
    return LHScene::onTouchBegan(touch, event); //always returns true
}

void LHSceneCutRope::onTouchMoved(Touch* touch, Event* event)
{
    //do you're own touch handling here - don't forget to call super
    LHScene::onTouchMoved(touch, event);
}

void LHSceneCutRope::onTouchEnded(Touch* touch, Event* event)
{
    //do you're own touch handling here - don't forget to call super
    LHScene::onTouchEnded(touch, event);
}

void LHSceneCutRope::onTouchCancelled(Touch* touch, Event* event)
{
    //do you're own touch handling here - don't forget to call super
    LHScene::onTouchCancelled(touch, event);
}