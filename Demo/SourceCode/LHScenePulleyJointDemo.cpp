
#include "LHScenePulleyJointDemo.h"

LHScenePulleyJointDemo* LHScenePulleyJointDemo::create()
{
    LHScenePulleyJointDemo *ret = new LHScenePulleyJointDemo();
    if (ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/pulleyJointDemo.lhplist"))
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

LHScenePulleyJointDemo::LHScenePulleyJointDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
#if LH_USE_BOX2D
    mouseJoint = NULL;
#else
    touchedNode = NULL;
#endif
    
}

LHScenePulleyJointDemo::~LHScenePulleyJointDemo()
{
    //nothing to release
    this->destroyMouseJoint();
}

std::string LHScenePulleyJointDemo::className(){
    return "LHScenePulleyJointDemo";
}

bool LHScenePulleyJointDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHSceneDemo::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    Size size = this->getContentSize();
    
    Label* ttf = Label::create();

#if LH_USE_BOX2D
    ttf->setString("PULLEY JOINT DEMO Example.\nDrag the weights around.");
#else
    ttf->setString("PULLEY JOINT DEMO Example.\nSorry, this demo is not available when using Chipmunk.\nPlease switch to Box2d target in Xcode.");
#endif
    
    
    ttf->setTextColor(Color4B::BLACK);
    ttf->setHorizontalAlignment(TextHAlignment::CENTER);
    ttf->setPosition(Point(size.width*0.5, size.height*0.5));
    ttf->setSystemFontSize(20);
    this->getUINode()->addChild(ttf);//add the text to the ui element as we dont want it to move with the camera
    
        
    return retValue;
}

void LHScenePulleyJointDemo::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
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
void LHScenePulleyJointDemo::onTouchesMoved(const std::vector<Touch*>& touches, Event* event){
    
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
void LHScenePulleyJointDemo::onTouchesEnded(const std::vector<Touch*>& touches, Event* event){
    
    this->destroyMouseJoint();
    
    LHScene::onTouchesEnded(touches, event);
}
void LHScenePulleyJointDemo::onTouchesCancelled(const std::vector<Touch*>& touches, Event *event){

    this->destroyMouseJoint();
    
    LHScene::onTouchesCancelled(touches, event);
}

void LHScenePulleyJointDemo::createMouseJointForTouchLocation(Point point)
{
#if LH_USE_BOX2D
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
    
#endif
}

void LHScenePulleyJointDemo::setTargetOnMouseJoint(Point point)
{
#if LH_USE_BOX2D
    if(mouseJoint == 0)
        return;
    
    b2Vec2 locationWorld = b2Vec2(this->metersFromPoint(point));
    mouseJoint->SetTarget(locationWorld);
#endif
}

void LHScenePulleyJointDemo::destroyMouseJoint()
{
#if LH_USE_BOX2D
    if(mouseJoint){
        this->getBox2dWorld()->DestroyJoint(mouseJoint);
    }
    mouseJoint = NULL;
#endif
    
}
