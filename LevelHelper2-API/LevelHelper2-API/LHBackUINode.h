//
//  LHBackUINode.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_BACK_UI_NODE_H__
#define __LEVELHELPER_API_BACK_UI_NODE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"

using namespace cocos2d;

/**
 LHBackUINode class is used to load a the UI objects from a level file.
 Users can retrieve node objects by calling the scene (LHScene) getChildNodeWithName("name") method.
 */

class LHDictionary;
class LHScene;

class LHBackUINode : public Node, public LHNodeProtocol
{
public:
 
    static LHBackUINode* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHBackUINode();
    virtual ~LHBackUINode();
    
    
    static  bool isLHBackUINode(Node* obj){return (0 != dynamic_cast<LHBackUINode*>(obj));}
    virtual bool isBackUINode(){return true;}    
};

#endif //__LEVELHELPER_API_BACK_UI_NODE_H__
