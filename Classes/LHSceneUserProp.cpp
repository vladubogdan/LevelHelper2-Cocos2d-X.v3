
#include "LHSceneUserProp.h"

USING_NS_CC;

LHSceneUserProp* LHSceneUserProp::createWithContentOfFile(const std::string& plistLevelFile)
{
    LHSceneUserProp *ret = new LHSceneUserProp();
    if (ret && ret->initWithContentOfFile(plistLevelFile))
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
LHSceneUserProp::LHSceneUserProp()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneUserProp::~LHSceneUserProp()
{
    //nothing to release
}

bool LHSceneUserProp::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHScene::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    
    LHSprite* blueRobot = (LHSprite*)this->getChildNodeWithName("blueRobot");
    
    if(blueRobot){
        DemoCustomClass* prop = (DemoCustomClass*)blueRobot->getUserProperty();
        CCLOG("ALIVE %d", prop->getAlive());
        CCLOG("STRING %s", prop->getStringMember().c_str());
        CCLOG("LIFE %f", prop->getLife());
        
        Node* otherRobot = prop->getOtherRobot();
        if(otherRobot){
            LHSprite* spr = (LHSprite*)otherRobot;
            if(spr){
                CCLOG("OTHER ROBOT NAME %s", spr->getName().c_str());
            }
        }
    }
    
    
//    LHAsset* assetInLevel = (LHAsset*)this->getChildNodeWithName("OfficerAsset");
//    if(assetInLevel){
//        printf("Found the following asset in the loaded level %p name %s\n", assetInLevel, assetInLevel->getName().c_str());
//    }
    
    
//    LHAsset* asset = LHAsset::createWithName("myNewAsset", "OfficerAsset.lhasset", this->getGameWorld());
//    if(asset){
//        asset->setPosition(Point(110,180));
//    }
    
//    LHSprite* imgSpr = LHSprite::createWithFile("bgrd.png", "DEMO_INDIVIDUAL_IMAGES_EXAMPLE/", this->getGameWorld());
//    if(imgSpr){
//        imgSpr->setAnchorPoint(Point(0,0));
//        imgSpr->setPosition(Point(0,0));
//        imgSpr->setLocalZOrder(-40);
//        CCLOG("DID CREATE SPRITE FROM IMAGE %p", imgSpr);
//        
//    }
    
//    LHSprite* frmSpr = LHSprite::createWithSpriteName("object_backpack",
//                                                      "objects.plist",
//                                                      "DEMO_PUBLISH_FOLDER/",
//                                                      this->getGameWorld());
//    if(frmSpr){
//        frmSpr->setPosition(Point(400,300));
//        CCLOG("DID CREATE SPRITE FROM SHEET %p", frmSpr);
//    }

    
    return retValue;
}