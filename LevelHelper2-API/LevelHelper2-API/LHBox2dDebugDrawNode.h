//
//  LHBox2dDebugDrawNode.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 02/07/14.
//
//

#ifndef __LEVELHELPER_API_BOX2D_DEBUG_NODE_H__
#define __LEVELHELPER_API_BOX2D_DEBUG_NODE_H__
#include "LHConfig.h"
#include "cocos2d.h"

using namespace cocos2d;


#if LH_USE_BOX2D

#include "Box2D/Box2D.h"

class LHBox2dDebugDrawNode;

struct b2AABB;
class LHBox2dDebug : public b2Draw
{
private:
    float mRatio;
    LHBox2dDebugDrawNode* drawNode;
public:
    
    
	LHBox2dDebug( float ratio, LHBox2dDebugDrawNode* drawNode );
    virtual ~LHBox2dDebug(){};
    
    void setRatio(float ratio);
    
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    
	void DrawTransform(const b2Transform& xf);
    
    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
    
    void DrawString(int x, int y, const char* string, ...);
    
    void DrawAABB(b2AABB* aabb, const b2Color& color);
};



class LHBox2dDebugDrawNode : public DrawNode
{
public:
    
    static LHBox2dDebugDrawNode* create();
    
    LHBox2dDebugDrawNode();
    virtual ~LHBox2dDebugDrawNode();
    bool init();

    static  bool isDebugDrawNode(Node* obj){return (0 != dynamic_cast<LHBox2dDebugDrawNode*>(obj));}
    
    LHBox2dDebug* getDebug();
    bool drawState(){return _drawState;}
    void setDrawState(bool val){_drawState = val;}
    
private:
    LHBox2dDebug*   _debug;
    bool _drawState;
    
};


#endif//lh_use_box2d

#endif //__LEVELHELPER_API_BOX2D_DEBUG_NODE_H__
