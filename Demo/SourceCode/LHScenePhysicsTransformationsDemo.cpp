
#include "LHScenePhysicsTransformationsDemo.h"

LHScenePhysicsTransformationsDemo* LHScenePhysicsTransformationsDemo::create()
{
    LHScenePhysicsTransformationsDemo *ret = new LHScenePhysicsTransformationsDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/physicsTransformationsDemo.lhplist"))
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

LHScenePhysicsTransformationsDemo::LHScenePhysicsTransformationsDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHScenePhysicsTransformationsDemo::~LHScenePhysicsTransformationsDemo()
{
    //nothing to release
}

std::string LHScenePhysicsTransformationsDemo::className(){
    return "LHScenePhysicsTransformationsDemo";
}

bool LHScenePhysicsTransformationsDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    
    Label* ttf = Label::create();
    
//#if LH_USE_BOX2D
    ttf->setString("PHYSICS TRANSFORMATIONS DEMO\nLevelHelper API takes care of setting the correct transformations when using physics.\nIt handles transformations in both directions from  Box2d to Cocos2d-X and from Cocos2d-X to Box2d.\nClick on a node to transform it with a random scale and rotation.");
//#else
//    ttf->setString("PHYSICS TRANSFORMATIONS DEMO\nWhen using default Cocos2d-X physics engine (Chipmunk),\nCocos2d-X handles the transformations from the physics engine to its own coordinate system.\nClick on a node to transform it with a random scale and rotation.\nNotice that scaling currently does not work.\nIn Xcode, switch to the Box2d target for complete transformations using the LevelHelper API.");
//#endif
    
#if LH_DEBUG == 0
    ttf->setString("This demo needs debug drawing enabled.\nGo to LHConfig.h and set LH_DEBUG = 1");
#endif
    
    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

void LHScenePhysicsTransformationsDemo::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    if(touches.size() < 1){
        return;
    }
    Touch* touch = touches[0];
    

    Point location = touch->getLocation();
    
    
    __Array* allPhysicalChildren = this->getGameWorldNode()->getChildrenOfType<Node*>();
    
    for(int i = 0; i < allPhysicalChildren->count(); ++i)
    {
        Node* node = (Node*)allPhysicalChildren->getObjectAtIndex(i);
        
        if(node && node->getBoundingBox().containsPoint(location))
        {
//            node->removeFromParent();//this will remove the node together with its physical body (if any)
            
            CCLOG("SETTING NODE %p TO LOCATION %f %f", node, location.x, location.y);
            node->setPosition(location);
            node->setRotation(LHUtils::LHRandomFloat(0, 360));
            node->setScaleX(LHUtils::LHRandomFloat(0.2, 1.5f));
            node->setScaleY(LHUtils::LHRandomFloat(0.2, 1.5f));
            return;
        }
    }
    //dont forget to call super
    LHScene::onTouchesBegan(touches, event);
}

