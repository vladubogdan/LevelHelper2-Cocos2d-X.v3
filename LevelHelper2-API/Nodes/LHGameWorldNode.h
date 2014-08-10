//
//  LHGameWorldNode.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_GAME_WORLD_NODE_H__
#define __LEVELHELPER_API_GAME_WORLD_NODE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHConfig.h"


#if LH_USE_BOX2D
class b2World;
class LHBox2dDebugDrawNode;
#endif

using namespace cocos2d;

/**
 LHGameWorldNode class is used to load the game world node object from a level file.
 Users can retrieve node objects by calling the scene (LHScene) getChildNodeWithName("name") method.
 */

class LHDictionary;
class LHScene;

class LHGameWorldNode : public Node, public LHNodeProtocol
{
public:
 
    static LHGameWorldNode* nodeWithDictionary(LHDictionary* dict, Node* prnt);

    LHGameWorldNode();
    virtual ~LHGameWorldNode();
    bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    static  bool isLHGameWorldNode(Node* obj){return (0 != dynamic_cast<LHGameWorldNode*>(obj));}
    virtual bool isGameWorldNode(){return true;}
    
#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif
    
    void update(float delta);
    
#if LH_USE_BOX2D
    b2World* getBox2dWorld();
    LHBox2dDebugDrawNode* _debugNode;
#endif
    
    Point getGravity();
    void setGravity(Point val);
    
private:

#if LH_USE_BOX2D
    void step(float dt);
    
    b2World* _box2dWorld;
#endif

    
};

#endif //__LEVELHELPER_API_GAME_WORLD_NODE_H__
