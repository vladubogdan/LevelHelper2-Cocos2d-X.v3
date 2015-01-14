
#include "LHSceneCameraDemo.h"

LHSceneCameraDemo* LHSceneCameraDemo::create()
{
    LHSceneCameraDemo *ret = new LHSceneCameraDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/cameraDemo.lhplist"))
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

LHSceneCameraDemo::LHSceneCameraDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneCameraDemo::~LHSceneCameraDemo()
{
    //nothing to release
}

std::string LHSceneCameraDemo::className(){
    return "LHSceneCameraDemo";
}

bool LHSceneCameraDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    Label* ttf = Label::create();
    
//#if LH_USE_BOX2D
    ttf->setString("CAMERA DEMO\nDemonstrate a simple camera that moves in a game world by an animation.\nThe camera is not restricted and does not follow any object.\nThe blue sky is added to the Back User Interface so it will always be on screen in the back.\nThis text is added in the Front User Interface node, so it will always be on screen.");
//#else
//    ttf->setString("CAMERA DEMO\nDemonstrate a simple camera that moves in a game world by an animation.\nThe camera is not restricted and does not follow any object.\nThe blue sky is added to the Back User Interface so it will always be on screen in the back.\nThis text is added in the Front User Interface node, so it will always be on screen.\nWhen using the default Cocos2d-X physics engine (Chipmunk)\ncamera works only when physics is not used.(BUG in Cocos2d-X)\nIn Xcode switch to the Box2d target to see the camera system at its full potential.\nCocos2d-X team promised to fix the bug in 3.2");
//#endif
    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

