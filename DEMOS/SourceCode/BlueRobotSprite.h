//
//  BlueRobotSprite.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 25/08/14.
//
//

#ifndef __LevelHelper2_Cocos2d_X_v3__BlueRobotSprite__
#define __LevelHelper2_Cocos2d_X_v3__BlueRobotSprite__

#include <iostream>
#include "LevelHelper2API.h"

class BlueRobotSprite : public LHSprite
{
public:
    
    static BlueRobotSprite* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    
    BlueRobotSprite();
    virtual ~BlueRobotSprite();
    
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    void printMessage();
};


#endif /* defined(__LevelHelper2_Cocos2d_X_v3__BlueRobotSprite__) */
