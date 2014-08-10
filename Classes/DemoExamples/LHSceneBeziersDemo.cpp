
#include "LHSceneBeziersDemo.h"

LHSceneBeziersDemo* LHSceneBeziersDemo::create()
{
    LHSceneBeziersDemo *ret = new LHSceneBeziersDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/beziersDemo.lhplist"))
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

LHSceneBeziersDemo::LHSceneBeziersDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneBeziersDemo::~LHSceneBeziersDemo()
{
    //nothing to release
}

std::string LHSceneBeziersDemo::className(){
    return "LHSceneBeziersDemo";
}

bool LHSceneBeziersDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    Label* ttf = Label::create();
    
    ttf->setString("BEZIERS DEMO\nBeziers can be used to draw line shapes.\nBy disabling control points you can have part of the bezier as a straight line.\nIn LevelHelper, select a bezier and hold control to edit it.Right click to toggle control points.\nYou can draw the outline of a shape using beziers and then make that outline into a shape.");

    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

