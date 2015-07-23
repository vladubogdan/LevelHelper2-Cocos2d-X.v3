
#include "LHSceneJointWithCameraBUGFix.h"

LHSceneJointWithCameraBUGFix* LHSceneJointWithCameraBUGFix::create()
{
    LHSceneJointWithCameraBUGFix *ret = new LHSceneJointWithCameraBUGFix();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/ropeWithCamera.lhplist"))
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

LHSceneJointWithCameraBUGFix::LHSceneJointWithCameraBUGFix()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneJointWithCameraBUGFix::~LHSceneJointWithCameraBUGFix()
{
    //nothing to release
}

std::string LHSceneJointWithCameraBUGFix::className(){
    return "LHSceneJointWithCameraBUGFix";
}

bool LHSceneJointWithCameraBUGFix::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    Label* ttf = Label::create();
    
    ttf->setString("Rope Joint With Camera Bug Fix.");

    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

