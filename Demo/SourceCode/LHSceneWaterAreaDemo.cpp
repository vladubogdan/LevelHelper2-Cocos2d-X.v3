
#include "LHSceneWaterAreaDemo.h"

LHSceneWaterAreaDemo* LHSceneWaterAreaDemo::create()
{
    LHSceneWaterAreaDemo *ret = new LHSceneWaterAreaDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/waterArea.lhplist"))
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

LHSceneWaterAreaDemo::LHSceneWaterAreaDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneWaterAreaDemo::~LHSceneWaterAreaDemo()
{
    //nothing to release
}

std::string LHSceneWaterAreaDemo::className(){
    return "LHSceneWaterAreaDemo";
}

bool LHSceneWaterAreaDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    Label* ttf = Label::create();
    
    
#if LH_USE_BOX2D
    ttf->setString("Water Area Example.\nObjects will float based on the water density property and physics density property of each object.\nIf water has a velocity value, objects will also be pushed in the direction of the velocity.\nTurbulence of the water can be bigger or smaller.\nSplashes can be disabled.\nWaves can be longer or shorter.\n\nDrag each object and throw it in the air to make a splash.");
#else
    ttf->setString("Water Area Example.\nSorry, this demo is not completely available using Chipmunk. Try Box2d instead.\n");
#endif

    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
        
    return retValue;
}