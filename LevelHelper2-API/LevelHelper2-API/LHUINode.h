//
//  LHUINode.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_UI_NODE_H__
#define __LEVELHELPER_API_UI_NODE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"

using namespace cocos2d;

/**
 LHUINode class is used to load a node object from a level file.
 Users can retrieve node objects by calling the scene (LHScene) getChildNodeWithName("name") method.
 */

class LHDictionary;
class LHScene;

class LHUINode : public Node, public LHNodeProtocol
{
public:
 
    static LHUINode* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHUINode();
    virtual ~LHUINode();

    
    
    static  bool isLHUINode(Node* obj){return (0 != dynamic_cast<LHUINode*>(obj));}
    virtual bool isUINode(){return true;}
    
};

#endif //__LEVELHELPER_API_UI_NODE_H__
