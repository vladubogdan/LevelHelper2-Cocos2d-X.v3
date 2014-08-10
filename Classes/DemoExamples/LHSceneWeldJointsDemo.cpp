
#include "LHSceneWeldJointsDemo.h"

LHSceneWeldJointsDemo* LHSceneWeldJointsDemo::create()
{
    LHSceneWeldJointsDemo *ret = new LHSceneWeldJointsDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/weldJointsDemo.lhplist"))
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

LHSceneWeldJointsDemo::LHSceneWeldJointsDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneWeldJointsDemo::~LHSceneWeldJointsDemo()
{
    //nothing to release
}

std::string LHSceneWeldJointsDemo::className(){
    return "LHSceneWeldJointsDemo";
}

bool LHSceneWeldJointsDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    
    Size size = this->getContentSize();
    
    Label* ttf = Label::create();
    
    
#if LH_USE_BOX2D
    ttf->setString("Weld Joints Demo.\n");
#else
    ttf->setString("Weld Joints Example.\nSorry, this demo is not available when using Chipmunk.\nPlease switch to Box2d target in Xcode.");
#endif
    
    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}
