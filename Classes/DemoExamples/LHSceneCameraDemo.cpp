
#include "LHSceneCameraDemo.h"

LHSceneCameraDemo* LHSceneCameraDemo::create()
{
    LHSceneCameraDemo *ret = new LHSceneCameraDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/tst.plist"))
    //if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/cameraDemo.plist"))
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
    ttf->setString("CAMERA DEMO\nDemonstrate a simple camera that moves in a game world by an animation.\nThe camera is not restricted and does not follow any object.\nThe blue sky is added to the Back User Interface so it will always be on screen in the back.\nThis text is added in the Front User Interface node, so it will always be on screen.");
    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

