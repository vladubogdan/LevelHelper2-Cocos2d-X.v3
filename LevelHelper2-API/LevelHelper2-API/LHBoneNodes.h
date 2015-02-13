//
//  LHBoneNodes.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_BONE_NODES_H__
#define __LEVELHELPER_API_BONE_NODES_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHNodeAnimationProtocol.h"
#include "LHConfig.h"

using namespace cocos2d;

/**
LHBoneNodes class serves as a container for all the nodes connected to a bone structure.
 */

class LHDictionary;
class LHScene;

class LHBoneNodes : public Node, public LHNodeProtocol
{
public:
 
    static LHBoneNodes* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHBoneNodes();
    virtual ~LHBoneNodes();
    
    static  bool isLHBoneNodes(Node* obj){return (0 != dynamic_cast<LHBoneNodes*>(obj));}
    virtual bool isBoneNodes(){return true;}
    
};

#endif //__LEVELHELPER_API_BONE_NODES_H__
