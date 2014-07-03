
#include "LHSceneParallaxDemo.h"

LHSceneParallaxDemo* LHSceneParallaxDemo::create()
{
    LHSceneParallaxDemo *ret = new LHSceneParallaxDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/parallaxDemo.plist"))
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

LHSceneParallaxDemo::LHSceneParallaxDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneParallaxDemo::~LHSceneParallaxDemo()
{
    //nothing to release
}

std::string LHSceneParallaxDemo::className(){
    return "LHSceneParallaxDemo";
}

bool LHSceneParallaxDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    
    Label* ttf = Label::create();
    ttf->setString("PARALLAX DEMO\nDemonstrate using a parallax to give an illusion of depth.\nThe camera is following the tire sprite.\nThe parallax is following the camera.\nEach layer in the parallax has a different movement ratio making them move at different speeds.\nLayers in the back move slower then layer in the front.\nThe blue sky is added to the Back User Interface so it will always be on screen in the back.\nThis text is added in the Front User Interface node, so it will always be on screen.");
    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

