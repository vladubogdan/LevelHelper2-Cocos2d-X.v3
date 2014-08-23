
#include "LHSceneIntroductionDemo.h"

LHSceneIntroductionDemo* LHSceneIntroductionDemo::create()
{
    LHSceneIntroductionDemo *ret = new LHSceneIntroductionDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/introductionDemo.lhplist"))
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

LHSceneIntroductionDemo::LHSceneIntroductionDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneIntroductionDemo::~LHSceneIntroductionDemo()
{
    //nothing to release
}

std::string LHSceneIntroductionDemo::className(){
    return "LHSceneIntroductionDemo";
}

bool LHSceneIntroductionDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    
    Label* ttf = Label::create();
    ttf->setString("INTRODUCTION\nUse the Previous and Next buttons to toggle between demos.\nUse the Restart button to start the current demo again.\nInvestigate each demo source file and LevelHelper document file for more info on how it was done.\nYou can find all scene files in the DEMO_DOCUMENTS\\levels folder.\nYou can find all source files in the DemoExamples folder located under Classes in Xcode.\nFor acurate FPS count use a real device and disable debug drawing in LHConfig.h.\nGo to AppDelegate.cpp to set your own starting scene.");
    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

