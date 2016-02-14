
#include "LHSceneWaterAreaDemo.h"

LHSceneWaterAreaDemo* LHSceneWaterAreaDemo::create()
{
    LHSceneWaterAreaDemo *ret = new LHSceneWaterAreaDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/waterArea.lhplist"))
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

LHSceneWaterAreaDemo::LHSceneWaterAreaDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneWaterAreaDemo::~LHSceneWaterAreaDemo()
{
#if LH_USE_BOX2D
    this->destroyMouseJoint();
#endif
    //nothing to release
}

std::string LHSceneWaterAreaDemo::className(){
    return "LHSceneWaterAreaDemo";
}

bool LHSceneWaterAreaDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    Label* ttf = Label::create();
    
    
#if LH_USE_BOX2D
    mouseJoint = NULL;
    
    ttf->setString("Water Area Example.\nObjects will float based on the water density property and physics density property of each object.\nIf water has a velocity value, objects will also be pushed in the direction of the velocity.\nTurbulence of the water can be bigger or smaller.\nSplashes can be disabled.\nWaves can be longer or shorter.\n\nDrag each object and throw it in the air to make a splash.");
#else
    ttf->setString("Water Area Example.\nSorry, this demo is not completely available using Chipmunk. Try Box2d instead.\n");
#endif

    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
        
    return retValue;
}

#if LH_USE_BOX2D
void LHSceneWaterAreaDemo::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    if(touches.size() < 1){
        return;
    }
    Touch* touch = touches[0];
    
    Point touchLocation = touch->getLocation();
    touchLocation = this->getGameWorldNode()->convertToNodeSpace(touchLocation);
    
    
    this->createMouseJointForTouchLocation(touchLocation);
    
    //dont forget to call super
    LHScene::onTouchesBegan(touches, event);
}
void LHSceneWaterAreaDemo::onTouchesMoved(const std::vector<Touch*>& touches, Event* event){
    
    if(touches.size() < 1){
        return;
    }
    Touch* touch = touches[0];
    
    Point touchLocation = touch->getLocation();
    touchLocation = this->getGameWorldNode()->convertToNodeSpace(touchLocation);
    
    
    this->setTargetOnMouseJoint(touchLocation);
    
    //dont forget to call super
    LHScene::onTouchesMoved(touches, event);
}
void LHSceneWaterAreaDemo::onTouchesEnded(const std::vector<Touch*>& touches, Event* event){
    
    this->destroyMouseJoint();
    
    LHScene::onTouchesEnded(touches, event);
}
void LHSceneWaterAreaDemo::onTouchesCancelled(const std::vector<Touch*>& touches, Event *event){
    
    this->destroyMouseJoint();
    
    LHScene::onTouchesCancelled(touches, event);
}

void LHSceneWaterAreaDemo::createMouseJointForTouchLocation(Point point)
{
    b2Body* ourBody = NULL;
    
    b2World* world = this->getBox2dWorld();
    
    if(world == NULL)return;
    
    LHNode* mouseJointDummyNode = (LHNode*)this->getChildNodeWithName("dummyBodyForMouseJoint");
    
    b2Body* mouseJointBody = mouseJointDummyNode->getBox2dBody();
    
    if(!mouseJointBody)return;
    
    b2Vec2 pointToTest = this->metersFromPoint(point);
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if(b != mouseJointBody)
        {
            b2Fixture* stFix = b->GetFixtureList();
            while(stFix != 0){
                if(stFix->TestPoint(pointToTest)){
                    if(ourBody == NULL)
                    {
                        ourBody = b;
                    }
                    else{
                        LHNode* ourNode = (LHNode*)(ourBody->GetUserData());
                        LHNode* bNode   = (LHNode*)(b->GetUserData());
                        
                        if(bNode->getLocalZOrder() > ourNode->getLocalZOrder())
                        {
                            ourBody = b;
                        }
                    }
                }
                stFix = stFix->GetNext();
            }
        }
    }
    
    if(ourBody == NULL || mouseJointBody == NULL)
        return;
    
    b2MouseJointDef md;
    md.bodyA = mouseJointBody;
    md.bodyB = ourBody;
    b2Vec2 locationWorld = pointToTest;
    
    md.target = locationWorld;
    md.collideConnected = true;
    md.maxForce = 1000.0f * ourBody->GetMass();
    ourBody->SetAwake(true);
    
    if(mouseJoint){
        world->DestroyJoint(mouseJoint);
        mouseJoint = NULL;
    }
    mouseJoint = (b2MouseJoint*)world->CreateJoint(&md);
}

void LHSceneWaterAreaDemo::setTargetOnMouseJoint(Point point)
{
    if(mouseJoint == 0)
        return;
    
    b2Vec2 locationWorld = b2Vec2(this->metersFromPoint(point));
    mouseJoint->SetTarget(locationWorld);
}

void LHSceneWaterAreaDemo::destroyMouseJoint()
{
    if(mouseJoint){
        this->getBox2dWorld()->DestroyJoint(mouseJoint);
    }
    mouseJoint = NULL;
}

#endif