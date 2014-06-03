#include "AppDelegate.h"
#include "HelloWorldScene.h"

#include "LHSceneSubclass.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
//    auto scene = HelloWorld::createScene();

//    auto scene = LHSceneSubclass::createWithContentOfFile("DEMO_PUBLISH_FOLDER/level01.plist");
//    auto scene = LHSceneSubclass::createWithContentOfFile("DEMO_PUBLISH_FOLDER/level02-anim.plist");
//    auto scene = LHSceneSubclass::createWithContentOfFile("DEMO_PUBLISH_FOLDER/level03-physics.plist");
//    auto scene = LHSceneSubclass::createWithContentOfFile("DEMO_PUBLISH_FOLDER/level03-characterAnimation.plist");
    
    //BUG IN COCOS2D-X v3.x please see discussion https://github.com/cocos2d/cocos2d-x/pull/5493
//    auto scene = LHSceneSubclass::createWithContentOfFile("DEMO_PUBLISH_FOLDER/level04-camera.plist");
    
//    auto scene = LHSceneSubclass::createWithContentOfFile("DEMO_PUBLISH_FOLDER/level05-shape.plist");
//    auto scene = LHSceneSubclass::createWithContentOfFile("DEMO_PUBLISH_FOLDER/level06-assets.plist");
    
    //Since physics and camera don't go well together in cocos2d-x because of the bug 5493 we simulate the parallax using an animation (for testing)
    //camera will have to wait until cocos2d-x team fixes the issue or until i add Box2d physics support.
//      auto scene = LHSceneSubclass::createWithContentOfFile("DEMO_PUBLISH_FOLDER/level07-parallax.plist");
    
    auto scene = LHSceneSubclass::createWithContentOfFile("DEMO_PUBLISH_FOLDER/level08-ropeJoint.plist");
    
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
