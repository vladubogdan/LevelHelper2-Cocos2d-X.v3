//
//  LHSprite.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 24/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_SPRITE_H__
#define __LEVELHELPER_API_SPRITE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
//#import "LHNodeAnimationProtocol.h"
//#import "LHUserPropertyProtocol.h"


using namespace cocos2d;

/**
 LHSprite class is used to load textured rectangles that are found in a level file.
 Users can retrieve a sprite object by calling the scene (LHScene) childNodeWithName: method.
 */

class LHDictionary;
class LHScene;

class LHSprite : public Sprite, public LHNodeProtocol  //<LHNodeProtocol, LHNodeAnimationProtocol>
{

public:

    static LHSprite* spriteNodeWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHSprite();
    virtual ~LHSprite();
    bool initWithDictionary(LHDictionary* dict, Node* prnt);

    static  bool isLHSprite(Node* obj){return (0 != dynamic_cast<LHSprite*>(obj));}
    virtual bool isSprite(){return true;}
private:

    /**
     Returns the unique identifier of this sprite node.
     */
//    -(NSString*)uuid;
    
    /**
     Returns all the tags of the node. (array with NSString's);
     */
//    -(NSArray*)tags;
    
    /**
     Returns the user property object assigned to this object or nil.
     */
//    -(id<LHUserPropertyProtocol>)userProperty;
//    -(void)setSpriteFrameWithName:(NSString*)spriteFrame;
    

};

#endif //__LEVELHELPER_API_SPRITE_H__//