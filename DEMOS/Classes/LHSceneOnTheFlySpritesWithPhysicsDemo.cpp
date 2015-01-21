
#include "LHSceneOnTheFlySpritesWithPhysicsDemo.h"

LHSceneOnTheFlySpritesWithPhysicsDemo* LHSceneOnTheFlySpritesWithPhysicsDemo::create()
{
    LHSceneOnTheFlySpritesWithPhysicsDemo *ret = new LHSceneOnTheFlySpritesWithPhysicsDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/emptyLevel.lhplist"))
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

LHSceneOnTheFlySpritesWithPhysicsDemo::LHSceneOnTheFlySpritesWithPhysicsDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneOnTheFlySpritesWithPhysicsDemo::~LHSceneOnTheFlySpritesWithPhysicsDemo()
{
    //nothing to release
}

std::string LHSceneOnTheFlySpritesWithPhysicsDemo::className(){
    return "LHSceneOnTheFlySpritesWithPhysicsDemo";
}

bool LHSceneOnTheFlySpritesWithPhysicsDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    
    Size size = this->getContentSize();
    
    Label* ttf = Label::create();
    
    
    ttf->setString("ON THE FLY SPRITES DEMO\nClick to create a sprite with a physics body as defined in the\nLevelHelper 2 Sprite Packer & Physics Editor tool.");

    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

void LHSceneOnTheFlySpritesWithPhysicsDemo::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    if(touches.size() < 1){
        return;
    }
    Touch* touch = touches[0];

    Point location = touch->getLocation();
    
    location = this->getGameWorldNode()->convertToNodeSpace(location);
    

    LHSprite* sprite = LHSprite::createWithSpriteName("carBody",
                                                      "carParts.png",
                                                      "DEMO_PUBLISH_FOLDER/",
                                                      this->getGameWorldNode());
    
    printf("did create %s %p\n", sprite->getName().c_str(), sprite);
    sprite->setPosition(location);
    
    
    //dont forget to call super
    LHScene::onTouchesBegan(touches, event);
}
