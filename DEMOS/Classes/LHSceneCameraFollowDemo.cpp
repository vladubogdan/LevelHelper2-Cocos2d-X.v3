
#include "LHSceneCameraFollowDemo.h"

LHSceneCameraFollowDemo* LHSceneCameraFollowDemo::create()
{
    LHSceneCameraFollowDemo *ret = new LHSceneCameraFollowDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/cameraFollowDemo.lhplist"))
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

LHSceneCameraFollowDemo::LHSceneCameraFollowDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneCameraFollowDemo::~LHSceneCameraFollowDemo()
{
    //nothing to release
}

std::string LHSceneCameraFollowDemo::className(){
    return "LHSceneCameraFollowDemo";
}

bool LHSceneCameraFollowDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    
    Label* ttf = Label::create();
    
//#if LH_USE_BOX2D
    ttf->setString("CAMERA FOLLOW DEMO\nDemonstrate a camera following an object (the tire sprite).\nThe camera is restricted and cannot go outside the game world rectangle.\nNotice how on the sides the candy will no longer be in the center and the camera stops following it.\nThe blue sky is added to the Back User Interface so it will always be on screen in the back.\nThis text is added in the Front User Interface node, so it will always be on screen.\nClick to change the gravity direction.\nScroll or pinch to zoom in/out.");
//#else
//    ttf->setString("CAMERA FOLLOW DEMO\nDemonstrate a camera following an object (the tire sprite).\nThe camera is restricted and cannot go outside the game world rectangle.\nNotice how on the sides the candy will no longer be in the center and the camera stops following it.\nThe blue sky is added to the Back User Interface so it will always be on screen in the back.\nThis text is added in the Front User Interface node, so it will always be on screen.\nClick to change the gravity direction.\nWhen using the default Cocos2d-X physics engine (Chipmunk)\ncamera works only when physics is not used.(BUG in Cocos2d-X)\nIn Xcode switch to the Box2d target to see the camera system at its full potential.\nCocos2d-X team promised to fix the bug in 3.2");
//#endif
    
    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

void LHSceneCameraFollowDemo::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    if(touches.size() < 1){
        return;
    }
    //Touch* touch = touches[0];
    
    Point curGravity = this->getGlobalGravity();
    if(didChangeX){
        this->setGlobalGravity(Point(curGravity.x, -curGravity.y));
        didChangeX = false;
    }
    else{
        didChangeX = true;
        this->setGlobalGravity(Point(-curGravity.x, curGravity.y));
    }

    //dont forget to call super
    LHScene::onTouchesBegan(touches, event);
}
