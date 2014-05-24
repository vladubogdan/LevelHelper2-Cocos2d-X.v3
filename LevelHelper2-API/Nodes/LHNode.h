//
//  LHNode.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_NODE_H__
#define __LEVELHELPER_API_NODE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHNodeAnimationProtocol.h"

using namespace cocos2d;

/**
 LHNode class is used to load a node object from a level file.
 Users can retrieve node objects by calling the scene (LHScene) getChildNodeWithName("name") method.
 */

class LHDictionary;
class LHScene;

class LHNode : public Node, public LHNodeProtocol, public LHNodeAnimationProtocol
{
public:
    
    static LHNode* nodeWithDictionary(LHDictionary* dict, Node* prnt);

    LHNode();
    virtual ~LHNode();
    bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    static  bool isLHNode(Node* obj){return (0 != dynamic_cast<LHNode*>(obj));}
    virtual bool isNode(){return true;}
    
    //for some reason cocos2d-x people decided to make "visit()" method final - so we use this one instead
    virtual void visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated);
};

#endif //__LEVELHELPER_API_NODE_H__