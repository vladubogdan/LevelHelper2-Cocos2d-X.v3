//
//  LHAsset.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHAsset.h"
#include "LHUtils.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHDevice.h"

LHAsset::LHAsset()
{
}

LHAsset::~LHAsset()
{
    
}

LHAsset* LHAsset::createWithName(const std::string& nm, const std::string& assetFileName, Node* prnt)
{
    LHAsset *ret = new LHAsset();
    if (ret && ret->initWithName(nm, assetFileName, prnt))
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

bool LHAsset::initWithName(const std::string& nm, const std::string& assetFileName, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        setName(nm);
        LHScene* scene = (LHScene*)prnt->getScene();
        
        __Dictionary* assetInfo = scene->assetInfoForFile(assetFileName);
        if(assetInfo)
        {
            prnt->addChild(this);
            
            LHArray* childrenInfo = (LHArray*)assetInfo->objectForKey("children");
            if(childrenInfo)
            {
                for(int i = 0; i < childrenInfo->count(); ++i)
                {
                    LHDictionary* childInfo = childrenInfo->dictAtIndex(i);
                    
                    Node* node = LHScene::createLHNodeWithDictionary(childInfo, this);
#pragma unused (node)
                }
            }
        }
        
        return true;
    }
    return false;
}

LHAsset* LHAsset::assetNodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHAsset *ret = new LHAsset();
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

bool LHAsset::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        
        LHScene* scene = (LHScene*)prnt->getScene();
        
        loadGenericInfoFromDictionary(dict);
        
        //physics body needs to be created before adding this node to the parent
        loadPhysicsFromDictionary(dict->dictForKey("nodePhysics"), (LHScene*)prnt->getScene());
        
        prnt->addChild(this);
        
        if(dict->objectForKey("size"))
            this->setContentSize(dict->sizeForKey("size"));
        
        Point unitPos   = dict->pointForKey("generalPosition");
        Point pos       = LHScene::positionForNode(this, unitPos);
        
        LHDictionary* devPositions = dict->dictForKey("devicePositions");
        if(devPositions)
        {
            std::string unitPosStr = LHDevice::devicePosition(devPositions, LH_SCREEN_RESOLUTION);
            
            if(unitPosStr.length()>0){
                Point unitPos = PointFromString(unitPosStr);
                pos = LHScene::positionForNode(this, unitPos);
            }
        }
        
        this->setOpacity(dict->floatForKey("alpha"));
        this->setRotation(dict->floatForKey("rotation"));
        this->setZOrder(dict->floatForKey("zOrder"));
        
        
        Point scl = dict->pointForKey("scale");
        this->setScaleX(scl.x);
        this->setScaleY(scl.y);
        
        
        Point anchor = dict->pointForKey("anchor");
        anchor.y = 1.0f - anchor.y;
        this->setAnchorPoint(anchor);
        
        this->setPosition(pos);
        
        if(dict->objectForKey("assetFile"))
        {
            LHDictionary* assetInfo = (LHDictionary*)scene->assetInfoForFile(dict->stringForKey("assetFile"));
            
            if(assetInfo)
            {
                LHArray* childrenInfo = assetInfo->arrayForKey("children");
                if(childrenInfo)
                {
                    for(int i = 0; i < childrenInfo->count(); ++i)
                    {
                        LHDictionary* childInfo = childrenInfo->dictAtIndex(i);

                        Node* node = LHScene::createLHNodeWithDictionary(childInfo, this);
                        #pragma unused (node)
                    }
                }
            }
            else{
                CCLOG("WARNING: COULD NOT FIND INFORMATION FOR ASSET %s.", this->getName().c_str());
            }
        }

        
        createAnimationsFromDictionary(dict);
        
        return true;
    }
    return false;
}

void LHAsset::visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated)
{
    visitActiveAnimation();
    Node::visit(renderer, parentTransform, parentTransformUpdated);
}

