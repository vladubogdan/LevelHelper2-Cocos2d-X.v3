//
//  CameraPhysicsTest

//
//

#include "CameraPhysicsTest.h"

USING_NS_CC;
CameraPhysicsTest* CameraPhysicsTest::create()
{
    CameraPhysicsTest* ret = new CameraPhysicsTest();
    if(ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/cameraPhysicsTest.lhplist"))
    {
        ret->autorelease();
        return ret;
    }
    else{
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

std::string CameraPhysicsTest::className(){
    return "CameraPhysicsTest";
}


CameraPhysicsTest::CameraPhysicsTest(){
    //initialize your content here
#if LH_USE_BOX2D
    mouseJoint = NULL;
#endif
    
}
CameraPhysicsTest::~CameraPhysicsTest(){
    //release your content here
    
    this->destroyMouseJoint();
}
bool CameraPhysicsTest::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool returnValue = LHScene::initWithContentOfFile(plistLevelFile);
    if(returnValue){
        //retrieve objects from level here
        
    
    }
    
    return returnValue;
}


void CameraPhysicsTest::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
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
void CameraPhysicsTest::onTouchesMoved(const std::vector<Touch*>& touches, Event* event){
    
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
void CameraPhysicsTest::onTouchesEnded(const std::vector<Touch*>& touches, Event* event){
    
    this->destroyMouseJoint();
    
    LHScene::onTouchesEnded(touches, event);
}
void CameraPhysicsTest::onTouchesCancelled(const std::vector<Touch*>& touches, Event *event){
    
    this->destroyMouseJoint();
    
    LHScene::onTouchesCancelled(touches, event);
}



void CameraPhysicsTest::createMouseJointForTouchLocation(Point point)
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

void CameraPhysicsTest::setTargetOnMouseJoint(Point point)
{
#if LH_USE_BOX2D
    if(mouseJoint == 0)
        return;
    
    b2Vec2 locationWorld = b2Vec2(this->metersFromPoint(point));
    
    mouseJoint->SetTarget(locationWorld);
#endif
}

void CameraPhysicsTest::destroyMouseJoint()
{
#if LH_USE_BOX2D
    if(mouseJoint){
        this->getBox2dWorld()->DestroyJoint(mouseJoint);
    }
    mouseJoint = NULL;
#endif
    
}