
#include "LHSceneComplexPhysicsDemo.h"

LHSceneComplexPhysicsDemo* LHSceneComplexPhysicsDemo::create()
{
    LHSceneComplexPhysicsDemo *ret = new LHSceneComplexPhysicsDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/complexPhysics.lhplist"))
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

LHSceneComplexPhysicsDemo::LHSceneComplexPhysicsDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneComplexPhysicsDemo::~LHSceneComplexPhysicsDemo()
{
    //nothing to release
}

std::string LHSceneComplexPhysicsDemo::className(){
    return "LHSceneComplexPhysicsDemo";
}

bool LHSceneComplexPhysicsDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    Label* ttf = Label::create();
    
    ttf->setString("COMPLEX PHYSICS DEMO\nShows body shapes created with the LevelHelper 2 integrated Physics Editor.");

    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

#if LH_USE_BOX2D
void LHSceneComplexPhysicsDemo::didBeginContact(const LHContactInfo& contact){
    
    printf("DID BEGIN CONTACT WITH INFO\n");
    printf("Node A %s\n", contact.nodeA->getName().c_str());
    printf("Node B %s\n", contact.nodeB->getName().c_str());
    printf("Node A Shape Name %s\n", contact.nodeAShapeName.c_str());
    printf("Node B Shape Name %s\n", contact.nodeBShapeName.c_str());
    printf("Node A Shape ID %d\n", contact.nodeAShapeID);
    printf("Node B Shape ID %d\n", contact.nodeBShapeID);
    printf("Impulse %f\n", contact.impulse);
    printf("Contact Point %f %f\n", contact.contactPoint.x, contact.contactPoint.y);
    printf("Box2d Contact Info %p\n", contact.box2dContact);
    printf("------------------------------------------\n");
}

void LHSceneComplexPhysicsDemo::didEndContact(const LHContactInfo& contact){

    printf("DID END CONTACT WITH INFO\n");
    printf("Node A %s\n", contact.nodeA->getName().c_str());
    printf("Node B %s\n", contact.nodeB->getName().c_str());
    printf("Node A Shape Name %s\n", contact.nodeAShapeName.c_str());
    printf("Node B Shape Name %s\n", contact.nodeBShapeName.c_str());
    printf("Node A Shape ID %d\n", contact.nodeAShapeID);
    printf("Node B Shape ID %d\n", contact.nodeBShapeID);
    printf("Box2d Contact Info %p\n", contact.box2dContact);
    printf("------------------------------------------\n");
}
#endif


