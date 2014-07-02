
#include "LHSceneDemo.h"
#include "LHSceneIntroductionDemo.h"
#include "LHSceneCameraDemo.h"
#include "LHSceneCameraFollowDemo.h"

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
    
    availableScenes["LHSceneIntroductionDemo"] = (createFuncPtrType)&LHSceneIntroductionDemo::create;
    availableScenes["LHSceneCameraDemo"] = (createFuncPtrType)&LHSceneCameraDemo::create;
    availableScenes["LHSceneCameraFollowDemo"] = (createFuncPtrType)&LHSceneCameraFollowDemo::create;
    
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
    

    typedef std::map<std::string, createFuncPtrType >::reverse_iterator it_type;
    
    for(it_type iterator = availableScenes.rbegin(); iterator != availableScenes.rend(); iterator++) {

        std::string className = iterator->first;
        std::string currentName = this->className();
        
        if(className == currentName)
        {
            it_type newIdx = iterator;
            newIdx++;
            if(newIdx == availableScenes.rend()){
                newIdx = availableScenes.rbegin();
            }

            
            std::string newClassname = newIdx->first;
            
            createFuncPtrType func = newIdx->second;
            
            LHSceneDemo* demo = func();
            
            Director::getInstance()->replaceScene(demo);
            return;
        }
    }

    
    
}

void LHSceneDemo::restartDemo(Ref *pSender, Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    typedef std::map<std::string, createFuncPtrType >::iterator it_type;
    
    for(it_type iterator = availableScenes.begin(); iterator != availableScenes.end(); iterator++) {
        
        std::string className = iterator->first;
        std::string currentName = this->className();
        
        if(className == currentName)
        {
            createFuncPtrType func = iterator->second;
            LHSceneDemo* demo = func();
            Director::getInstance()->replaceScene(demo);
            return;
        }
    }
}

void LHSceneDemo::nextDemo(Ref *pSender, Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) return;
    
    typedef std::map<std::string, createFuncPtrType >::iterator it_type;
    
    for(it_type iterator = availableScenes.begin(); iterator != availableScenes.end(); iterator++) {

        std::string className = iterator->first;
        std::string currentName = this->className();

        if(className == currentName)
        {
            it_type newIdx = iterator;
            newIdx++;
            if(newIdx == availableScenes.end()){
                newIdx = availableScenes.begin();
            }
            
            std::string newClassname = newIdx->first;
            
            createFuncPtrType func = newIdx->second;

            LHSceneDemo* demo = func();
            
            Director::getInstance()->replaceScene(demo);
            return;
        }
    }
}