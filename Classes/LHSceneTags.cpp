
#include "LHSceneTags.h"

USING_NS_CC;

LHSceneTags* LHSceneTags::createWithContentOfFile(const std::string& plistLevelFile)
{
    LHSceneTags *ret = new LHSceneTags();
    if (ret && ret->initWithContentOfFile(plistLevelFile))
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
LHSceneTags::LHSceneTags()
{
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT NOTHING IS LOADED*/
}

LHSceneTags::~LHSceneTags()
{
    //nothing to release
}

bool LHSceneTags::initWithContentOfFile(const std::string& plistLevelFile)
{
    bool retValue = LHScene::initWithContentOfFile(plistLevelFile);
    
    /*INITIALIZE YOUR CONTENT HERE*/
    /*AT THIS POINT EVERYTHING IS LOADED AND YOU CAN ACCESS YOUR OBJECTS*/
    

    CCLOG("SEARCH FOR BANANAS ...................");
    //this should return only the banana sprites
    {
        std::vector<std::string>tagsToLookFor;
        tagsToLookFor.push_back("BANANA");
        __Array* bananas = this->getChildrenWithTags(tagsToLookFor);
        
        if(bananas)
        {
            for(size_t i = 0; i < bananas->count(); ++i){
                LHSprite* spr = (LHSprite*)bananas->getObjectAtIndex(i);
                CCLOG("SPRITE NAME %s", spr->getName().c_str());
            }
        }
    }
    
    CCLOG("SEARCH FOR BANANAS AND PINEAPPLE...................");
    //this should return only the banana and the pineapple sprites
    {
        std::vector<std::string>tagsToLookFor;
        tagsToLookFor.push_back("BANANA");
        tagsToLookFor.push_back("PINEAPPLE");
        bool any = true;
        __Array* objects = this->getChildrenWithTags(tagsToLookFor, any);
        
        if(objects)
        {
            for(size_t i = 0; i < objects->count(); ++i){
                LHSprite* spr = (LHSprite*)objects->getObjectAtIndex(i);
                CCLOG("SPRITE NAME %s", spr->getName().c_str());
            }
        }
    }

    
    CCLOG("SEARCH FOR BANANAS AND PINEAPPLE BY DISABLING ANY - SHOULD RETURN EMPTY ARRAY...................");
    //this should return only the banana and the pineapple sprites
    {
        std::vector<std::string>tagsToLookFor;
        tagsToLookFor.push_back("BANANA");
        tagsToLookFor.push_back("PINEAPPLE");
        bool any = false;
        __Array* objects = this->getChildrenWithTags(tagsToLookFor, any);
        
        if(objects)
        {
            for(size_t i = 0; i < objects->count(); ++i){
                LHSprite* spr = (LHSprite*)objects->getObjectAtIndex(i);
                CCLOG("SPRITE NAME %s", spr->getName().c_str());
            }
        }
    }

    
    CCLOG("SEARCH FOR FRUITS THAT ARE BANANA - SHOULD RETURN ONLY BANANA AND NO PINEAPPLE (even though pineapple is a fruit)...................");
    //this should return only the banana and the pineapple sprites
    {
        std::vector<std::string>tagsToLookFor;
        tagsToLookFor.push_back("FRUIT");
        tagsToLookFor.push_back("BANANA");
        bool any = false;
        __Array* objects = this->getChildrenWithTags(tagsToLookFor, any);
        
        if(objects)
        {
            for(size_t i = 0; i < objects->count(); ++i){
                LHSprite* spr = (LHSprite*)objects->getObjectAtIndex(i);
                CCLOG("SPRITE NAME %s", spr->getName().c_str());
            }
        }
    }

    
    
    
    return retValue;
}