
#include "LHSceneAssetDemo.h"

LHSceneAssetDemo* LHSceneAssetDemo::create()
{
    LHSceneAssetDemo *ret = new LHSceneAssetDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/assetDemo.lhplist"))
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
    
    
//#if LH_USE_BOX2D
    ttf->setString("ASSETS DEMO\nAssets are special objects that when edited they will change\nto the new edited state everywhere they are used in your project.\nClick to create a new officer (asset) of a random scale and rotation.");
//#else
//    ttf->setString("ASSETS DEMO\nAssets are special objects that when edited they will change\nto the new edited state everywhere they are used in your project.\nClick to create a new officer (asset) of a random scale and rotation.\nChipmunk detected:\nSorry but currently Cocos2d-X has a bug where it does not update children physics body position.\nWhen using Chipmunk and having physics bodies\non children of the node that gets transformed the bodies will not get updated.\nSwitch to the Box2d target for correct physics transformations.\nCheck LHSceneAssetDemo source code for a possible work around.");
//#endif

    
    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

void LHSceneAssetDemo::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    if(touches.size() < 1){
        return;
    }
    Touch* touch = touches[0];
    
    Point location = touch->getLocation();
    
    LHAsset* asset = LHAsset::createWithName("myNewAsset",
                                             "OfficerAsset.lhasset",
                                             this->getGameWorldNode());

    float newScale = LHUtils::LHRandomFloat(0.15, 0.8f);
    float zRot = LHUtils::LHRandomFloat(-45, 45.0f);

    //for chipmunk possible fix - comment this next 4 lines and uncomment the if statement below
    asset->setPosition(location);
    asset->setScaleX(newScale);
    asset->setScaleY(newScale);
    asset->setRotation(zRot);

#if LH_USE_BOX2D == 0 //chipmunk fix
    /*A possible fix for chipmunk-cocos2d bug will be to set the transformations on the node that has a physics body.
     Our asset contains the node "Officer" which has a dynamic body. So instead of setting the position to the asset node, like we do
     for box2d we may set the transformations directly on the node containing the body - e.g "officer".
     Hower - changing scale will not work as cocos2d/chipmunk does not support that - another bug - or maybe by design.
     
     In order to make it work uncomment the following lines and comment the 4 lines above
     */
//    LHNode* node = (LHNode*)asset->getChildNodeWithName("Officer");
//    if(node){
//        node->setPosition(location);
//        node->setScaleX(newScale);
//        node->setScaleY(newScale);
//        node->setRotation(zRot);
//    }
#endif

    
    //dont forget to call super
    return LHScene::onTouchesBegan(touches, event);
}
