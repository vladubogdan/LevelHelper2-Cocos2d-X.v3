
#include "LHSceneCharacterAnimationDemo.h"

LHSceneCharacterAnimationDemo* LHSceneCharacterAnimationDemo::create()
{
    LHSceneCharacterAnimationDemo *ret = new LHSceneCharacterAnimationDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/characterAnimationDemo.lhplist"))
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

LHSceneCharacterAnimationDemo::LHSceneCharacterAnimationDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneCharacterAnimationDemo::~LHSceneCharacterAnimationDemo()
{
    //nothing to release
}

std::string LHSceneCharacterAnimationDemo::className(){
    return "LHSceneCharacterAnimationDemo";
}

bool LHSceneCharacterAnimationDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    
    Label* ttf = Label::create();
    ttf->setString("CHARACTER ANIMATION DEMO\nDemonstrate a character animation.\nThis demo also uses per device positioning.\nChange the device and run this demo again\nto see how the character is placed in a different position on each device.\nPer device positioning is mostly useful for User Interface elements,\nlike a life bar that you always want to be displayed in the top right corner.");
    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}


void LHSceneCharacterAnimationDemo::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    if(touches.size() < 1){
        return;
    }
//    Touch* touch = touches[0];
    
//    if(Director::getInstance()->isPaused())
//    {
//        Director::getInstance()->resume();
//    }
//    else{
//        Director::getInstance()->pause();
//    }
    
    
    LHNode* officerNode = (LHNode*)this->getChildNodeWithName("Officer");
    
    if(officerNode)
    {
        LHAnimation* anim = officerNode->getActiveAnimation();
        anim->setAnimating(!anim->animating());
        CCLOG("ANIMATION: %s %s.", anim->animating() ? "Playing" : "Pausing", anim->name().c_str());
    }

    
    //dont forget to call super
    LHScene::onTouchesBegan(touches, event);
}