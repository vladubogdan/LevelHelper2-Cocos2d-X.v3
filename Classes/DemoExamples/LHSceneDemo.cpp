
#include "LHSceneDemo.h"
#include "LHSceneIntroductionDemo.h"
#include "LHSceneCameraDemo.h"
#include "LHSceneCameraFollowDemo.h"
#include "LHSceneParallaxDemo.h"
#include "LHSceneCharacterAnimationDemo.h"
#include "LHSceneAssetDemo.h"
#include "LHSceneRopeJointDemo.h"

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

    sceneNames.push_back("LHSceneCameraDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneCameraDemo::create);

    sceneNames.push_back("LHSceneCameraFollowDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneCameraFollowDemo::create);

    sceneNames.push_back("LHSceneParallaxDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneParallaxDemo::create);

    sceneNames.push_back("LHSceneCharacterAnimationDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneCharacterAnimationDemo::create);

    sceneNames.push_back("LHSceneAssetDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneAssetDemo::create);

    sceneNames.push_back("LHSceneRopeJointDemo");
    sceneCreators.push_back((createFuncPtrType)&LHSceneRopeJointDemo::create);

    
    
    Size size = this->getContentSize();
    {
        Button* button = Button::create();
        button->setTitleText("Previous Demo");
        button->setPosition(Point(size.width*0.5 - 200, size.height - 50));
        //buttons do not work without a texture so we use this dummy empty texture
        button->loadTextures("emptyButtonTexture.png", "emptyButtonTexture.png", "");
        button->setTitleFontSize(32);
        button->setTitleColor(ccColor3B::MAGENTA);
        button->setEnabled(true);
        button->setTouchEnabled(true);
        button->addTouchEventListener(CC_CALLBACK_2(LHSceneDemo::previousDemo, this));
        this->addChild(button);
    }

    {
        Button* button = Button::create();
        button->setTitleText("Restart");
        button->setPosition(Point(size.width*0.5, size.height - 50));
        //buttons do not work without a texture so we use this dummy empty texture
        button->loadTextures("emptyButtonTexture.png", "emptyButtonTexture.png", "");
        button->setTitleFontSize(32);
        button->setTitleColor(ccColor3B::MAGENTA);
        button->setEnabled(true);
        button->setTouchEnabled(true);
        button->addTouchEventListener(CC_CALLBACK_2(LHSceneDemo::restartDemo, this));
        this->addChild(button);
    }

    {
        Button* button = Button::create();
        button->setTitleText("Next Demo");
        button->setPosition(Point(size.width*0.5 + 200, size.height - 50));
        //buttons do not work without a texture so we use this dummy empty texture
        button->loadTextures("emptyButtonTexture.png", "emptyButtonTexture.png", "");
        button->setTitleFontSize(32);
        button->setTitleColor(ccColor3B::MAGENTA);
        button->setEnabled(true);
        button->setTouchEnabled(true);
        button->addTouchEventListener(CC_CALLBACK_2(LHSceneDemo::nextDemo, this));
        this->addChild(button);
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
            int newIdx = i-1;
            if(newIdx < 0){
                newIdx = sceneNames.size()-1;
            }
            
            createFuncPtrType func = sceneCreators[newIdx];
            LHSceneDemo* demo = func();
            Director::getInstance()->replaceScene(demo);
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
            int newIdx = i+1;
            if(newIdx >= sceneNames.size()){
                newIdx = 0;
            }
            
            createFuncPtrType func = sceneCreators[newIdx];
            LHSceneDemo* demo = func();
            Director::getInstance()->replaceScene(demo);
            return;
        }
    }
}