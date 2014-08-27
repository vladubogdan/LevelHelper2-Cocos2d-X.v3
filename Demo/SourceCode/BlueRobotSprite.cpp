//
//  BlueRobotSprite.cpp
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 25/08/14.
//
//

#include "BlueRobotSprite.h"

BlueRobotSprite* BlueRobotSprite::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    BlueRobotSprite *ret = new BlueRobotSprite();
    if (ret && ret->initWithDictionary(dict, prnt))
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

bool BlueRobotSprite::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHSprite::initWithDictionary(dict, prnt);
    
    if(status){
        //retrieve elements from the LHSprite object here
        
        printf("Did create node of type BlueRobotSprite with name %s.\n", this->getName().c_str());
    }
    
    return status;
}

void BlueRobotSprite::printMessage()
{
    printf("I'm a BlueRobotSprite object.\n");
}

BlueRobotSprite::BlueRobotSprite(){
    
}

BlueRobotSprite::~BlueRobotSprite(){
    
}
