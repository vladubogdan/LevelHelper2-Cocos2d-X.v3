
#include "LHSceneRemoveOnCollisionDemo.h"

LHSceneRemoveOnCollisionDemo* LHSceneRemoveOnCollisionDemo::create()
{
    LHSceneRemoveOnCollisionDemo *ret = new LHSceneRemoveOnCollisionDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/removalOnCollisionDemo.lhplist"))
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

LHSceneRemoveOnCollisionDemo::LHSceneRemoveOnCollisionDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneRemoveOnCollisionDemo::~LHSceneRemoveOnCollisionDemo()
{
    //nothing to release
}

std::string LHSceneRemoveOnCollisionDemo::className(){
    return "LHSceneRemoveOnCollisionDemo";
}

bool LHSceneRemoveOnCollisionDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    Label* ttf = Label::create();
    
#if LH_USE_BOX2D
    ttf->setString("REMOVE OBJECTS ON COLLISION\nIf you are familiar with Box2d then you will know that\nremoving a body in the collision callback function\nwill make Box2d library assert as the world is locked.\nThe LevelHelper API solves this by sending the callbacks when its safe.\nCut the rope to remove the bodies when collision occurs.");
#else
    ttf->setString("This demo is currently not available when using Chipmunk.\nIn Xcode, please switch to the Box2d targets.");
#endif

    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
        
    return retValue;
}


void LHSceneRemoveOnCollisionDemo::handleCandyCollisionWithNode(Node* candy, Node* node)
{
    LHNodeProtocol* prot = LHNodeProtocol::LHGetNodeProtocol(node);
    if(prot)
    {
        if(prot->containsTag("BANANA"))
        {
            node->removeFromParent();
        }
        
    }
}

bool LHSceneRemoveOnCollisionDemo::disableCandyCollisionWithNode(Node* node)
{
    LHNodeProtocol* prot = LHNodeProtocol::LHGetNodeProtocol(node);
    if(prot)
    {
        if(prot->containsTag("BANANA"))
        {
            return true;
        }
    }
    return false;
}

#if LH_USE_BOX2D
bool LHSceneRemoveOnCollisionDemo::shouldDisableContactBetweenNodes(Node* nodeA, Node* nodeB){
    
#if COCOS2D_VERSION >= 0x00030200
    
    if(nodeA && nodeA->getName() == "candy")
    {
        return this->disableCandyCollisionWithNode(nodeB);
    }
    else
    {
        return this->disableCandyCollisionWithNode(nodeA);
    }
    
    return false;
    
#else
    
    //ensure that the nodes received comes from a LevelHelper API object
//    LHNodeProtocol* a = dynamic_cast<LHNodeProtocol*>(nodeA);
//    LHNodeProtocol* b = dynamic_cast<LHNodeProtocol*>(nodeB);
    
    LHNodeProtocol* a = LHNodeProtocol::LHGetNodeProtocol(nodeA);
    LHNodeProtocol* b = LHNodeProtocol::LHGetNodeProtocol(nodeB);
    
    
    if(a && b)
    {
        if(a->getName() == "candy")
        {
            return this->disableCandyCollisionWithNode(b);
        }
        else
        {
            return this->disableCandyCollisionWithNode(a);
        }                
    }
        
#endif
            
        
    return false;
}

void LHSceneRemoveOnCollisionDemo::didBeginContact(const LHContactInfo& contact)
{
    Node* nodeA = contact.nodeA;
    Node* nodeB = contact.nodeB;
    
#if COCOS2D_VERSION >= 0x00030200
    
    CCLOG("DID BEGIN %p %p", nodeA, nodeB);
    
    
    if(nodeA && nodeA->getName() == "candy")
    {
        this->handleCandyCollisionWithNode(nodeA, nodeB);
    }
    else
    {
        this->handleCandyCollisionWithNode(nodeB, nodeA);
    }
#else
    //ensure that the nodes received comes from a LevelHelper API object
    LHNodeProtocol* a = dynamic_cast<LHNodeProtocol*>(nodeA);
    LHNodeProtocol* b = dynamic_cast<LHNodeProtocol*>(nodeB);
    
    if(a->getName() == "candy")
    {
        this->handleCandyCollisionWithNode(nodeA, nodeB);
    }
    else
    {
        this->handleCandyCollisionWithNode(nodeB, nodeA);
    }
#endif
    
}

#else//chipmunk

bool LHSceneRemoveOnCollisionDemo::onContactBegin(PhysicsContact& contact){
    CCLOG("COLLISION DEMO CONTACT BEGIN");
    return true;
}

#endif
