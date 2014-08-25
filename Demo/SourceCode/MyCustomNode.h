//
//  MyCustomNode.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 25/08/14.
//
//

#ifndef __LevelHelper2_Cocos2d_X_v3__MyCustomNode__
#define __LevelHelper2_Cocos2d_X_v3__MyCustomNode__

#include <iostream>
#include "LevelHelper2API.h"

class MyCustomNode : public LHNode
{
public:
    
    static MyCustomNode* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    
    MyCustomNode();
    virtual ~MyCustomNode();
    
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    void printCustomNodeMessage();
    
    void addChildRepresentation();
};


#endif /* defined(__LevelHelper2_Cocos2d_X_v3__MyCustomNode__) */
