
#include "LHSceneSkeletalCharacterDemo.h"

LHSceneSkeletalCharacterDemo* LHSceneSkeletalCharacterDemo::create()
{
    LHSceneSkeletalCharacterDemo *ret = new LHSceneSkeletalCharacterDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/skeletalCharacter.lhplist"))
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

LHSceneSkeletalCharacterDemo::LHSceneSkeletalCharacterDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneSkeletalCharacterDemo::~LHSceneSkeletalCharacterDemo()
{
    //nothing to release
}

std::string LHSceneSkeletalCharacterDemo::className(){
    return "LHSceneSkeletalCharacterDemo";
}

bool LHSceneSkeletalCharacterDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    
    Size size = this->getContentSize();
    
    Label* ttf = Label::create();
    
    
    ttf->setString("Bone Structure Demo.");

    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

