//
//  LHBone.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_BONE_H__
#define __LEVELHELPER_API_BONE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHNodeAnimationProtocol.h"
#include "LHConfig.h"

using namespace cocos2d;

/**
 LHBone class is used to load a bone object from a level file.
 Users can retrieve node objects by calling the scene (LHScene) getChildNodeWithName("name") method.
 */

class LHDictionary;
class LHScene;
class LHBoneNodes;

class LHBone : public Node, public LHNodeProtocol, public LHNodeAnimationProtocol
{
public:
 
    static LHBone* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHBone();
    virtual ~LHBone();
    
    static  bool isLHBone(Node* obj){return (0 != dynamic_cast<LHBone*>(obj));}
    virtual bool isBone(){return true;}
    
    
    float getMaxAngle(){return _maxAngle;}
    float getMinAngle(){return _minAngle;}
    bool  getIsRigid(){return _rigid;}
    
    bool isRoot();
    LHBone* getRootBone();
    LHBoneNodes* getRootBoneNodes();

    
    

#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif
    
    
    virtual void setPosition(const cocos2d::Vec2 &pos);
    virtual void setRotation(float rotation);
    
private:
    float _maxAngle;
    float _minAngle;
    bool _rigid;
    __Array* _connections;
    
    void transformConnectedSprites();    
};

#endif //__LEVELHELPER_API_BONE_H__
