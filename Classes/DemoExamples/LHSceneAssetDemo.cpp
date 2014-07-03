
#include "LHSceneAssetDemo.h"

LHSceneAssetDemo* LHSceneAssetDemo::create()
{
    LHSceneAssetDemo *ret = new LHSceneAssetDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/assetDemo.plist"))
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

LHSceneAssetDemo::LHSceneAssetDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneAssetDemo::~LHSceneAssetDemo()
{
    //nothing to release
}

std::string LHSceneAssetDemo::className(){
    return "LHSceneAssetDemo";
}

bool LHSceneAssetDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    
    Label* ttf = Label::create();
    ttf->setString("ASSETS DEMO\nAssets are special objects that when edited they will change\nto the new edited state everywhere they are used in your project.\n\nClick to create a new officer (asset) of a random scale and rotation.");
    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

float randomFloat(float Min, float Max){
    return ((arc4random()%RAND_MAX)/(RAND_MAX*1.0))*(Max-Min)+Min;
}

bool LHSceneAssetDemo::onTouchBegan(Touch* touch, Event* event)
{
    Point location = touch->getLocation();
    
    CCLOG("LOCATION %f %f", location.x, location.y);
    
    
    LHAsset* asset = LHAsset::createWithName("myNewAsset",
                                             "OfficerAsset.lhasset",
                                             this->getGameWorldNode());
    
    asset->setPosition(location);
    
    asset->setScaleX(randomFloat(0.15, 0.8f));
    asset->setScaleY(randomFloat(0.15, 0.8f));
    
    float zRot = randomFloat(-45, 45.0f);
    asset->setRotation(zRot);
    
    //dont forget to call super
    return LHScene::onTouchBegan(touch, event);
}
