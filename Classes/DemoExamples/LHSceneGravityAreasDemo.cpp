
#include "LHSceneGravityAreasDemo.h"

LHSceneGravityAreasDemo* LHSceneGravityAreasDemo::create()
{
    LHSceneGravityAreasDemo *ret = new LHSceneGravityAreasDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/gravityAreas.lhplist"))
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

LHSceneGravityAreasDemo::LHSceneGravityAreasDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneGravityAreasDemo::~LHSceneGravityAreasDemo()
{
    //nothing to release
}

std::string LHSceneGravityAreasDemo::className(){
    return "LHSceneGravityAreasDemo";
}

bool LHSceneGravityAreasDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    Label* ttf = Label::create();
    
    ttf->setString("GRAVITY AREAS DEMO\nObjects inside the gravity areas will get a radial or directional velocity.");

    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

