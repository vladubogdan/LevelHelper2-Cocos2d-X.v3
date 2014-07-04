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
#include "LHPhysicsProtocol.h"


using namespace cocos2d;

/**
 LHAsset class is used to load an asset object from a level file or from the resources folder.
 Users can retrieve node objects by calling the scene (LHScene) childNodeWithName: method.
 */

class LHDictionary;

class LHAsset : public Node, public LHNodeProtocol, public LHNodeAnimationProtocol, public LHPhysicsProtocol
{
public:
    
    /**
     Creates a new asset node with a specific name.
     @param nm The name of the new asset node. Can be used later to retrieve the asset from the children hierarchy.
     @param assetFileName The name of the asset file. Do not provide an extension. E.g If file is named "myAsset.lhasset.plist" then yous should pass @"myAsset.lhasset".
     @param prnt The parent node. Must not be nil and must be a children of the LHScene (or subclass of LHScene).
     @return A new asset node.
     * @code
     * //this is how you should use this function
     * LHAsset* asset = LHAsset::createWithName("myNewAsset", "OfficerAsset.lhasset", this->getGameWorld());
     * asset->setPosition(Point(110,40));
     * @endcode
     */
    static LHAsset* createWithName(const std::string& nm, const std::string& assetFileName, Node* prnt);
    
    static LHAsset* assetNodeWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHAsset();
    virtual ~LHAsset();
    
    static  bool isLHAsset(Node* obj){return (0 != dynamic_cast<LHAsset*>(obj));}
    virtual bool isAsset(){return true;}
  
    //for some reason cocos2d-x people decided to make "visit()" method final - so we use this one instead
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
    
#if LH_USE_BOX2D
    virtual void removeFromParent();
    virtual void setPosition(const cocos2d::Vec2 &pos);
    virtual void setRotation(float rotation);
    
    virtual void setScaleX(float scaleX);
    virtual void setScaleY(float scaleY);
    virtual void setScale(float scaleX, float scaleY);
    
    virtual void updatePosition(const cocos2d::Vec2 &pos);
    virtual void updateRotation(float rotation);
#endif

    
private:
    
    bool initWithName(const std::string& nm, const std::string& assetFileName, Node* prnt);
    bool initWithDictionary(LHDictionary* dict, Node* prnt);
};

#endif //__LEVELHELPER_API_ASSET_H__
