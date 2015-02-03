
#include "LHSceneCollisionHandlingDemo.h"

LHSceneCollisionHandlingDemo* LHSceneCollisionHandlingDemo::create()
{
    LHSceneCollisionHandlingDemo *ret = new LHSceneCollisionHandlingDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/collisionHandlingDemo.lhplist"))
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

LHSceneCollisionHandlingDemo::LHSceneCollisionHandlingDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneCollisionHandlingDemo::~LHSceneCollisionHandlingDemo()
{
    //nothing to release
}

std::string LHSceneCollisionHandlingDemo::className(){
    return "LHSceneCollisionHandlingDemo";
}

bool LHSceneCollisionHandlingDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    Label* ttf = Label::create();
    
    
#if LH_USE_BOX2D
    ttf->setString("COLLISION DEMO\nWatch the console for collision information.\nCheck the LHSceneCollisionDemo.mm for more info.\n\nWhen the car tyre will enter the gravity area it will be thrown upwards.\nIf the position of the car tyre is under the wood object collision will be disabled.\nWhen its on top of it, collision will occur.");
#else
    ttf->setString("COLLISION DEMO\nWhen using Chipmunk we use the Cocos2d-X implementation for collision handling.");
#endif

    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
        
    return retValue;
}

#if LH_USE_BOX2D
bool LHSceneCollisionHandlingDemo::shouldDisableContactBetweenNodes(Node* nodeA, Node* nodeB){
    
#if COCOS2D_VERSION >= 0x00030200
    
    if(nodeA && nodeB)
    {
        CCLOG("SHOULD DISABLE CONTACT BETWEEN %s and %s", nodeA->getName().c_str(), nodeB->getName().c_str());
        
        if(
           (nodeA->getName() == "carTyre" && nodeB->getName() == "shouldNotCollide") ||
           (nodeB->getName() == "carTyre" && nodeA->getName() == "shouldNotCollide")
           )
        {
            if(nodeA->getName() == "carTyre")
            {
                if(nodeA->getPosition().y < nodeB->getPosition().y){
                    return true;
                }
            }
            
            if(nodeB->getName() == "carTyre")
            {
                if(nodeB->getPosition().y < nodeA->getPosition().y){
                    return true;
                }
            }
        }
    }
    
#else

        
    //ensure that the nodes received comes from a LevelHelper API object
    LHNodeProtocol* a = dynamic_cast<LHNodeProtocol*>(nodeA);
    LHNodeProtocol* b = dynamic_cast<LHNodeProtocol*>(nodeB);
    
    if(a && b)
    {
        CCLOG("SHOULD DISABLE CONTACT BETWEEN %s and %s", a->getName().c_str(), b->getName().c_str());
        
        if(
           (a->getName() == "carTyre" && b->getName() == "shouldNotCollide") ||
           (b->getName() == "carTyre" && a->getName() == "shouldNotCollide")
           )
        {
            if(a->getName() == "carTyre")
            {
                if(nodeA->getPosition().y < nodeB->getPosition().y){
                    return true;
                }
            }
            
            if(b->getName() == "carTyre")
            {
                if(nodeB->getPosition().y < nodeA->getPosition().y){
                    return true;
                }
            }
        }
    }
        
#endif
            
        
    return false;
}
void LHSceneCollisionHandlingDemo::didBeginContact(LHContactInfo contact)
{
    printf("DID BEGIN CONTACT %s %s scenePt %f %f impulse %f\n", contact.nodeA->getName().c_str(), contact.nodeB->getName().c_str(),
          contact.contactPoint.x, contact.contactPoint.y, contact.impulse);
}
void LHSceneCollisionHandlingDemo::didEndContact(LHContactInfo contact)
{
    printf("DID END CONTACT BETWEEN A:%s AND B:%s\n", contact.nodeA->getName().c_str(), contact.nodeB->getName().c_str());
}
#else

bool LHSceneCollisionHandlingDemo::onContactBegin(PhysicsContact& contact){
    CCLOG("COLLISION DEMO CONTACT BEGIN");
    return true;
}
#endif
