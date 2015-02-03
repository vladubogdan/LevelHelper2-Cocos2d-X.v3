
#include "LHSceneAssetWithJointsDemo.h"

LHSceneAssetWithJointsDemo* LHSceneAssetWithJointsDemo::create()
{
    LHSceneAssetWithJointsDemo *ret = new LHSceneAssetWithJointsDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/simpleCar.lhplist"))
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

LHSceneAssetWithJointsDemo::LHSceneAssetWithJointsDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneAssetWithJointsDemo::~LHSceneAssetWithJointsDemo()
{
    //nothing to release
}

std::string LHSceneAssetWithJointsDemo::className(){
    return "LHSceneAssetWithJointsDemo";
}

bool LHSceneAssetWithJointsDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    
    Size size = this->getContentSize();
    
    Label* ttf = Label::create();
    
    
#if LH_USE_BOX2D
    ttf->setString("CAR ASSETS DEMO\nAnother asset demo. This time demonstrating an asset containing joints.\nClick to create a new car of a random rotation.");
#else
    ttf->setString("CAR ASSETS DEMO\nSorry this demo is not available when using Chipmunk.\nPlease switch to the Box2d target inside Xcode.");
#endif

    
    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

void LHSceneAssetWithJointsDemo::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    if(touches.size() < 1){
        return;
    }
    Touch* touch = touches[0];

    Point location = touch->getLocation();
    
    LHAsset* asset = LHAsset::createWithName("myNewAsset",
                                             "carAsset.lhasset",
                                             this->getGameWorldNode());
    
    float zRot = LHUtils::LHRandomFloat(-45, 45.0f);
    asset->setPosition(location);
    asset->setRotation(zRot);

    //NOTE: you should not scale nodes containig joints or nodes that are connected to joints.
    //The joints will break or will have strange behaviour..
    //The only way to use scale is to scale the node prior creating the joint - so from inside LevelHelper 2 app.
    
    //dont forget to call super
    LHScene::onTouchesBegan(touches, event);
}
