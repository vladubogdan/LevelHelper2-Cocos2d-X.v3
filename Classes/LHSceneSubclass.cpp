#include "LHSceneSubclass.h"

USING_NS_CC;


Scene* LHSceneSubclass::createScene()
{
    CCLOG("LH SCENE SUBCLASS CREATE SCENE");
    
    CCLOG("DID CREATE SCENE\n");
    
    LHSceneSubclass *ret = new LHSceneSubclass();
    if (ret && ret->initWithContentOfFile("LH2-Published/example.lhplist"))
    {
        CCLOG("DID INIT OK\n");
        
        ret->autorelease();
        return ret;
    }
    else
    {
        CCLOG("SOMETHING WENT WRONG\n");
        
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

LHSceneSubclass::LHSceneSubclass()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneSubclass::~LHSceneSubclass()
{
    //nothing to release
}

bool LHSceneSubclass::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHScene::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    
    Size size = this->getContentSize();
    

    {
        Label* ttf = Label::create();
        ttf->setString("Welcome to");
        
        ttf->setTextColor(Color4B::BLACK);
        ttf->setHorizontalAlignment(TextHAlignment::CENTER);
        ttf->setPosition(Point(size.width*0.5, size.height*0.5+120));
        ttf->setSystemFontSize(40);
        this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    }
    {
        Label* ttf = Label::create();
        ttf->setString("LevelHelper 2");
        
        ttf->setTextColor(Color4B::BLACK);
        ttf->setHorizontalAlignment(TextHAlignment::CENTER);
        ttf->setPosition(Point(size.width*0.5, size.height*0.5+60));
        ttf->setSystemFontSize(80);
        this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    }
    {
        Label* ttf = Label::create();
        ttf->setString("Run the DEMO targets for examples.\nCheck LHSceneSubclass.cpp to learn how to load a level.\nVisit www.gamedevhelper.com for more learn resources.");
        
        ttf->setTextColor(Color4B::BLACK);
        ttf->setHorizontalAlignment(TextHAlignment::CENTER);
        ttf->setPosition(Point(size.width*0.5, size.height*0.5-60));
        ttf->setSystemFontSize(20);
        this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    }

    
    return retValue;
}
