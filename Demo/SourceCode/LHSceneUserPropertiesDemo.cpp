
#include "LHSceneUserPropertiesDemo.h"

LHSceneUserPropertiesDemo* LHSceneUserPropertiesDemo::create()
{
    LHSceneUserPropertiesDemo *ret = new LHSceneUserPropertiesDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/userPropertiesDemo.lhplist"))
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

LHSceneUserPropertiesDemo::LHSceneUserPropertiesDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneUserPropertiesDemo::~LHSceneUserPropertiesDemo()
{
    //nothing to release
}

std::string LHSceneUserPropertiesDemo::className(){
    return "LHSceneUserPropertiesDemo";
}

bool LHSceneUserPropertiesDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    Label* ttf = Label::create();
    
    ttf->setString("USER PROPERTIES ON A NODE\nDemonstrate using user properties that were setup inside LevelHelper 2.\nLook at the console for the output.\nLook inside LHSceneUserPropertiesDemo.cpp for help on how you can set this up.");
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.3));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera

    
    /*HELP
     
     //Creating the property class
     1. Inside LevelHelper 2 go to "Tools" menu and choose "Properties Manager"
     2. Write a "Class Name" and click enter to create that class.
     3. With the class selected write a "Member Name" and click enter to create that property.
     4. In the list of properties choose the type of property you want (number, string, connection, boolean)
     //connection is a way to connect one node to another for fast referencing.
     5. Set the "Code Generation Path" to the LHUserProperties.h file that comes with LH 2 api if not already set.
     
     //Assigning the property class on a node
     1. Inside the "Level Editor" select the node you want to assign a property to.
     2. Go to the "Properties Inspector" tab - the first tab on the right side.
     3. Further down you will find "User Info Properties".
     4. Select the property class you want from the "User Info" list.
     5. Assign any value you want on the members you have setup for that property class.
     
     Further down you will see how you can access the properties you have setup.
     First you search for the node that has the property assign to it.
     Then you get the userProperty pointer and convert it to the right property class.
     Then its a matther of reading/writing the property members.
     */
    
    
    
#ifdef __LH_USER_PROPERTY_ROBOTUSERPROPERTY__
    //we test for this define here as the user might have this class inside its own project which may have
    //different properties or no properties defined. We dont want a compilation error if that happens.
    
    
    LHSprite* pinkRobot = (LHSprite*)this->getChildNodeWithName("pinkRobot");
    if(pinkRobot){
        RobotUserProperty* prop = (RobotUserProperty*)pinkRobot->getUserProperty();
        CCLOG("...............Pink Robot - User properites.................");
        CCLOG("LIFE (number property) %f", prop->getLife());
        CCLOG("ACTIVATED (boolean property) %d", prop->getActivated());
        CCLOG("MODEL (string property) %s", prop->getModel().c_str());
        
        Node* connectedRobot = prop->getConnection();
        if(connectedRobot){
            LHSprite* spr = (LHSprite*)connectedRobot;
            if(spr){
                CCLOG("CONNECTED ROBOT NAME (connection property) %s", spr->getName().c_str());
            }
        }
    }

    
    LHSprite* blueRobot = (LHSprite*)this->getChildNodeWithName("blueRobot");
    if(blueRobot){
        RobotUserProperty* prop = (RobotUserProperty*)blueRobot->getUserProperty();
        CCLOG("...............Blue Robot - User properites.................");
        CCLOG("LIFE (number property) %f", prop->getLife());
        CCLOG("ACTIVATED (boolean property) %d", prop->getActivated());
        CCLOG("MODEL (string property) %s", prop->getModel().c_str());
        
        Node* connectedRobot = prop->getConnection();
        if(connectedRobot){
            LHSprite* spr = (LHSprite*)connectedRobot;
            if(spr){
                CCLOG("CONNECTED ROBOT NAME (connection property) %s", spr->getName().c_str());
            }
        }
    }

    
    LHSprite* greenRobot = (LHSprite*)this->getChildNodeWithName("greenRobot");
    if(greenRobot){
        RobotUserProperty* prop = (RobotUserProperty*)greenRobot->getUserProperty();
        CCLOG("...............Green Robot - User properites.................");
        CCLOG("LIFE (number property) %f", prop->getLife());
        CCLOG("ACTIVATED (boolean property) %d", prop->getActivated());
        CCLOG("MODEL (string property) %s", prop->getModel().c_str());
        
        Node* connectedRobot = prop->getConnection();
        if(connectedRobot){
            LHSprite* spr = (LHSprite*)connectedRobot;
            if(spr){
                CCLOG("CONNECTED ROBOT NAME (connection property) %s", spr->getName().c_str());
            }
        }
    }

#endif
    
    
        
    return retValue;
}

