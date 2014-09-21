//
//  DynamicObjectsTransformAnimationTest

//
//

#include "DynamicObjectsTransformAnimationTest.h"

USING_NS_CC;
DynamicObjectsTransformAnimationTest* DynamicObjectsTransformAnimationTest::create()
{
    DynamicObjectsTransformAnimationTest* ret = new DynamicObjectsTransformAnimationTest();
    if(ret && ret->initWithContentOfFile("DEMO_PUBLISH_FOLDER/dynamicObjAnimTest.lhplist"))
    {
        ret->autorelease();
        return ret;
    }
    else{
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

std::string DynamicObjectsTransformAnimationTest::className(){
    return "DynamicObjectsTransformAnimationTest";
}


DynamicObjectsTransformAnimationTest::DynamicObjectsTransformAnimationTest(){
    //initialize your content here

}
DynamicObjectsTransformAnimationTest::~DynamicObjectsTransformAnimationTest(){
    //release your content here
}
bool DynamicObjectsTransformAnimationTest::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool returnValue = LHScene::initWithContentOfFile(plistLevelFile);
    if(returnValue){
        //retrieve objects from level here
        
    
    }
    
    return returnValue;
}
