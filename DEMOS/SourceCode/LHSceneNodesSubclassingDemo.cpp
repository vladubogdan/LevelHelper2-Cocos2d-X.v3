
#include "LHSceneNodesSubclassingDemo.h"
#include "BlueRobotSprite.h"
#include "MyCustomNode.h"

LHSceneNodesSubclassingDemo* LHSceneNodesSubclassingDemo::create()
{
    LHSceneNodesSubclassingDemo *ret = new LHSceneNodesSubclassingDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/subclassingDemo.lhplist"))
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

LHSceneNodesSubclassingDemo::LHSceneNodesSubclassingDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneNodesSubclassingDemo::~LHSceneNodesSubclassingDemo()
{
    //nothing to release
}

std::string LHSceneNodesSubclassingDemo::className(){
    return "LHSceneNodesSubclassingDemo";
}

bool LHSceneNodesSubclassingDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    Label* ttf = Label::create();
    
    ttf->setString("NODES SUBLCASSING DEMO\nAll node types available in LevelHelper can be subclassed in order to add your own game logic.\nCheck LHSceneNodesSubclassingDemo for how to do it.\nBlue robot is of class \"BlueRobotSprite\" while the pink robot is a generic \"LHSprite\" class.\nThe node is of class \"MyCustomNode\" and the blue outline is draw by the custom class.\nClick the screen and watch the console for a message from each subclassed object.");

    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
    return retValue;
}

LevelHelperNodeTypeSubclass LHSceneNodesSubclassingDemo::createNodeObjectForSubclassWithName(const std::string subclassTypeName, const std::string lhTypeName)
{
    CCLOG("DID ASK TO CREATE OBJECT OF TYPE %s subclassed from %s", subclassTypeName.c_str(), lhTypeName.c_str());
    
    if(subclassTypeName == "BlueRobotSprite")
        return (LevelHelperNodeTypeSubclass)&BlueRobotSprite::nodeWithDictionary;
    
    if(subclassTypeName == "MyCustomNode")
        return (LevelHelperNodeTypeSubclass)&MyCustomNode::nodeWithDictionary;
    
    
    return nullptr;
}

void LHSceneNodesSubclassingDemo::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    if(touches.size() < 1){
        return;
    }
    Touch* touch = touches[0];
    
    Point location = touch->getLocation();
    
    __Array* allChildrenOfTypeBlueRobotSprite = this->getGameWorldNode()->getChildrenOfType<BlueRobotSprite*>();
    for(int i = 0; i < allChildrenOfTypeBlueRobotSprite->count(); ++i)
    {
        BlueRobotSprite* node = (BlueRobotSprite*)allChildrenOfTypeBlueRobotSprite->getObjectAtIndex(i);
        if(node)
        {
            //printMessage is a function available in BlueRobotSprite class
            node->printMessage();
        }
    }
    
    MyCustomNode* customNode = (MyCustomNode*)this->getChildNodeWithName("testCustomNode");
    if(customNode){
        customNode->printCustomNodeMessage();
    }

    //dont forget to call super
    LHScene::onTouchesBegan(touches, event);
}


