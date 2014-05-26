//
//  LHAsset.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_ASSET_H__
#define __LEVELHELPER_API_ASSET_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHNodeAnimationProtocol.h"

using namespace cocos2d;

/**
 LHAsset class is used to load an asset object from a level file or from the resources folder.
 Users can retrieve node objects by calling the scene (LHScene) childNodeWithName: method.
 */

class LHDictionary;

class LHAsset : public Node, public LHNodeProtocol, public LHNodeAnimationProtocol
{
public:
    
    static LHAsset* assetNodeWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHAsset();
    virtual ~LHAsset();
    bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    static  bool isLHAsset(Node* obj){return (0 != dynamic_cast<LHAsset*>(obj));}
    virtual bool isAsset(){return true;}
  
    //for some reason cocos2d-x people decided to make "visit()" method final - so we use this one instead
    virtual void visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated);
    
private:
    
};

#endif //__LEVELHELPER_API_ASSET_H__
