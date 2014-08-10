
#include "LHSceneParallaxDemo.h"

LHSceneParallaxDemo* LHSceneParallaxDemo::create()
{
    LHSceneParallaxDemo *ret = new LHSceneParallaxDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/parallaxDemo.lhplist"))
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
    
//#if LH_USE_BOX2D
    ttf->setString("PARALLAX DEMO\nDemonstrate using a parallax to give an illusion of depth.\nThe camera is following the tire sprite.\nThe parallax is following the camera.\nEach layer in the parallax has a different movement ratio making them move at different speeds.\nLayers in the back move slower then layer in the front.\nThe blue sky is added to the Back User Interface so it will always be on screen in the back.\nThis text is added in the Front User Interface node, so it will always be on screen.");
//#else
//     ttf->setString("PARALLAX DEMO\nDemonstrate using a parallax to give an illusion of depth.\nThe camera is following the tire sprite.\nThe parallax is following the camera.\nEach layer in the parallax has a different movement ratio making them move at different speeds.\nLayers in the back move slower then layer in the front.\nThe blue sky is added to the Back User Interface so it will always be on screen in the back.\nThis text is added in the Front User Interface node, so it will always be on screen.\nWhen using the default Cocos2d-X physics engine (Chipmunk)\ncamera works only when physics is not used.(BUG in Cocos2d-X)\nIn Xcode switch to the Box2d target to see the camera system at its full potential.\nCocos2d-X team promised to fix the bug in 3.2");
//#endif
    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

