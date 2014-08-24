
#include "LHSceneRopeJointDemo.h"

LHSceneRopeJointDemo* LHSceneRopeJointDemo::create()
{
    LHSceneRopeJointDemo *ret = new LHSceneRopeJointDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/ropeJointDemo.lhplist"))
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

LHSceneRopeJointDemo::LHSceneRopeJointDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneRopeJointDemo::~LHSceneRopeJointDemo()
{
    //nothing to release
}

std::string LHSceneRopeJointDemo::className(){
    return "LHSceneRopeJointDemo";
}

bool LHSceneRopeJointDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    
    Label* ttf = Label::create();
    ttf->setString("ROPE JOINTS DEMO\nThe left most joint is thinner and cannot be cut.\nThe middle joint does not use a texture.\nThe right most joint can be cut - Make a line to cut it.");
    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

void LHSceneRopeJointDemo::didCutRopeJoint(LHRopeJointNode* jt){
    
    CCLOG("DID CUT ROPE JOINT %s", jt->getName().c_str());
}
