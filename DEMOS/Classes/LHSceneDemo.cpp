
#include "LHSceneDemo.h"
#include "LHSceneIntroductionDemo.h"
#include "LHSceneCameraDemo.h"
#include "LHSceneCameraFollowDemo.h"
#include "LHSceneParallaxDemo.h"
#include "LHSceneCharacterAnimationDemo.h"
#include "LHSceneAssetDemo.h"
#include "LHSceneRopeJointDemo.h"
#include "LHSceneShapesDemo.h"
#include "LHScenePhysicsTransformationsDemo.h"
#include "LHSceneBeziersDemo.h"
#include "LHSceneCollisionHandlingDemo.h"
#include "LHSceneGravityAreasDemo.h"
#include "LHSceneUserPropertiesDemo.h"
#include "LHSceneAssetWithJointsDemo.h"
#include "LHScenePulleyJointDemo.h"
#include "LHSceneGearJointDemo.h"
#include "LHSceneWeldJointsDemo.h"
#include "LHSceneRemoveOnCollisionDemo.h"
#include "LHSceneWaterAreaDemo.h"
#include "LHSceneNodesSubclassingDemo.h"
#include "LHSceneComplexPhysicsDemo.h"
#include "LHSceneOnTheFlySpritesWithPhysicsDemo.h"
#include "LHSceneSkeletalCharacterDemo.h"
//TESTS
#include "CameraPhysicsTest.h"
#include "DynamicObjectsTransformAnimationTest.h"

#include "LHSceneJointWithCameraBUGFix.h"
LHSceneDemo* LHSceneDemo::create()
{
    printf("THIS CLASS SHOULD NOT BE INSTANTIATED DIRECTLY - USE A SUBCLASS\n");
    return nullptr;
}
LHSceneDemo::LHSceneDemo()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneDemo::~LHSceneDemo()
{
    //nothing to release
}

bool LHSceneDemo::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHScene::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    
    sceneNames.push_back("LHSceneIntroductionDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneIntroductionDemo::create);

//    sceneNames.push_back("LHSceneJointWithCameraBUGFix");
//    sceneCreators.push_back((createFuncPtrType)&LHSceneJointWithCameraBUGFix::create);
    
    sceneNames.push_back("LHSceneSkeletalCharacterDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneSkeletalCharacterDemo::create);

    sceneNames.push_back("LHSceneOnTheFlySpritesWithPhysicsDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneOnTheFlySpritesWithPhysicsDemo::create);
    
    sceneNames.push_back("LHSceneComplexPhysicsDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneComplexPhysicsDemo::create);
    
    //tests
//    sceneNames.push_back("DynamicObjectsTransformAnimationTest");
//    sceneCreators.push_back((createFuncPtrType)&DynamicObjectsTransformAnimationTest::create);
//    sceneNames.push_back("CameraPhysicsTest");
//    sceneCreators.push_back((createFuncPtrType)&CameraPhysicsTest::create);
    
    sceneNames.push_back("LHSceneNodesSubclassingDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneNodesSubclassingDemo::create);

    
    sceneNames.push_back("LHSceneCameraDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneCameraDemo::create);

    sceneNames.push_back("LHSceneCameraFollowDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneCameraFollowDemo::create);

    sceneNames.push_back("LHSceneParallaxDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneParallaxDemo::create);

    sceneNames.push_back("LHSceneCharacterAnimationDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneCharacterAnimationDemo::create);

    sceneNames.push_back("LHSceneRopeJointDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneRopeJointDemo::create);

    sceneNames.push_back("LHSceneWeldJointsDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneWeldJointsDemo::create);
    
    sceneNames.push_back("LHSceneGearJointDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneGearJointDemo::create);
    
    sceneNames.push_back("LHScenePulleyJointDemo");
    sceneCreators.push_back((createFuncPtrType)&LHScenePulleyJointDemo::create);
    
    sceneNames.push_back("LHSceneAssetDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneAssetDemo::create);
    
    sceneNames.push_back("LHSceneAssetWithJointsDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneAssetWithJointsDemo::create);

    sceneNames.push_back("LHSceneShapesDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneShapesDemo::create);
    
    sceneNames.push_back("LHSceneBeziersDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneBeziersDemo::create);

    sceneNames.push_back("LHScenePhysicsTransformationsDemo");
    sceneCreators.push_back((createFuncPtrType)&LHScenePhysicsTransformationsDemo::create);

    sceneNames.push_back("LHSceneCollisionHandlingDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneCollisionHandlingDemo::create);
    
    sceneNames.push_back("LHSceneRemoveOnCollisionDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneRemoveOnCollisionDemo::create);

    sceneNames.push_back("LHSceneGravityAreasDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneGravityAreasDemo::create);

    sceneNames.push_back("LHSceneUserPropertiesDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneUserPropertiesDemo::create);

    sceneNames.push_back("LHSceneWaterAreaDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneWaterAreaDemo::create);

    
    Size size = this->getContentSize();
    {
        Button* button = Button::create();
        button->setTitleText("Previous Demo");
        button->setPosition(Point(size.width*0.5 - 200, size.height - 50));
        //buttons do not work without a texture so we use this dummy empty texture
        button->loadTextures("emptyButtonTexture.png", "emptyButtonTexture.png", "");
        button->setTitleFontSize(32);
        button->setTitleColor(Color3B::MAGENTA);
        button->setEnabled(true);
        button->setTouchEnabled(true);
        button->addTouchEventListener(CC_CALLBACK_2(LHSceneDemo::previousDemo, this));
        this->getUINode()->addChild(button);
    }

    {
        Button* button = Button::create();
        button->setTitleText("Restart");
        button->setPosition(Point(size.width*0.5, size.height - 50));
        //buttons do not work without a texture so we use this dummy empty texture
        button->loadTextures("emptyButtonTexture.png", "emptyButtonTexture.png", "");
        button->setTitleFontSize(32);
        button->setTitleColor(Color3B::MAGENTA);
        button->setEnabled(true);
        button->setTouchEnabled(true);
        button->addTouchEventListener(CC_CALLBACK_2(LHSceneDemo::restartDemo, this));
        this->getUINode()->addChild(button);
    }

    {
        Button* button = Button::create();
        button->setTitleText("Next Demo");
        button->setPosition(Point(size.width*0.5 + 200, size.height - 50));
        //buttons do not work without a texture so we use this dummy empty texture
        button->loadTextures("emptyButtonTexture.png", "emptyButtonTexture.png", "");
        button->setTitleFontSize(32);
        button->setTitleColor(Color3B::MAGENTA);
        button->setEnabled(true);
        button->setTouchEnabled(true);
        button->addTouchEventListener(CC_CALLBACK_2(LHSceneDemo::nextDemo, this));
        this->getUINode()->addChild(button);
    }
    
    return retValue;
}

void LHSceneDemo::previousDemo(Ref *pSender, Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    

    for(size_t i = 0; i < sceneNames.size(); ++i)
    {
        std::string className  = sceneNames[i];
        std::string currentName= this->className();
        
        if(className == currentName)
        {
            int newIdx = (int)i-1;
            if(newIdx < 0){
                newIdx = (int)sceneNames.size()-1;
            }
            
            createFuncPtrType func = sceneCreators[newIdx];
            LHSceneDemo* demo = func();
            Director::getInstance()->replaceScene(demo);
//            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, demo));
            return;
        }
    }
}

void LHSceneDemo::restartDemo(Ref *pSender, Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    for(size_t i = 0; i < sceneNames.size(); ++i)
    {
        std::string className  = sceneNames[i];
        std::string currentName= this->className();
        
        if(className == currentName)
        {
            createFuncPtrType func = sceneCreators[i];
            LHSceneDemo* demo = func();
            Director::getInstance()->replaceScene(demo);
//            Director::getInstance()->replaceScene(TransitionSplitCols::create(1.0f, demo));
            
            return;
        }
    }
}

void LHSceneDemo::nextDemo(Ref *pSender, Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    for(size_t i = 0; i < sceneNames.size(); ++i)
    {
        std::string className  = sceneNames[i];
        std::string currentName= this->className();

        if(className == currentName)
        {
            int newIdx = (int)i+1;
            if(newIdx >= (int)sceneNames.size()){
                newIdx = 0;
            }
            
            createFuncPtrType func = sceneCreators[newIdx];
            LHSceneDemo* demo = func();
            Director::getInstance()->replaceScene(demo);
            
//            TransitionSceneOriented
//            TransitionRotoZoom
//            TransitionJumpZoom
//            TransitionMoveInL
//            TransitionMoveInR
//            TransitionMoveInT
//            TransitionMoveInB
//            TransitionSlideInL
//            TransitionSlideInR
//            TransitionSlideInB
//            TransitionSlideInT
//            TransitionShrinkGrow
//            TransitionFlipX
//            TransitionFlipY
//            TransitionFlipAngular
//            TransitionZoomFlipX
//            TransitionZoomFlipY
//            TransitionZoomFlipAngular
//            TransitionFade
//            TransitionCrossFade
//            TransitionTurnOffTiles
//            TransitionSplitCols
//            TransitionSplitRows
//            TransitionFadeTR
//            TransitionFadeBL
//            TransitionFadeUp
//            TransitionFadeDown
            
            Director::getInstance()->replaceScene(TransitionFade::create(0.4f, demo));
            return;
        }
    }
}