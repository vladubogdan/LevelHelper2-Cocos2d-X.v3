//
//  LHBox2dDebugDrawNode.cpp
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 02/07/14.
//
//

#include "LHBox2dDebugDrawNode.h"
#include "LHGameWorldNode.h"
#include "LHConfig.h"

#if LH_USE_BOX2D

LHBox2dDebug::LHBox2dDebug( float ratio, LHBox2dDebugDrawNode* node )
: mRatio( ratio ){
    drawNode = node;
}
void LHBox2dDebug::setRatio(float ratio){
    mRatio = ratio;
}

void LHBox2dDebug::DrawPolygon(const b2Vec2* old_vertices, int32 vertexCount, const b2Color& color)
{
    Vec2* vertices = new Vec2[vertexCount];
    for( int i=0;i<vertexCount;i++) {
		b2Vec2 tmp = old_vertices[i];
		tmp *= mRatio;
		vertices[i] = Vec2(tmp.x, tmp.y);
	}
    
    Color4F fillColor = Color4F(color.r, color.g, color.b, 0.5);
    Color4F borderColor = Color4F(color.r, color.g, color.b, 1);
    
    drawNode->drawPolygon(vertices, vertexCount, fillColor, 1, borderColor);
    
    delete[] vertices;
}

void LHBox2dDebug::DrawSolidPolygon(const b2Vec2* old_vertices, int32 vertexCount, const b2Color& color)
{
    Vec2* vertices = new Vec2[vertexCount];
    
    for( int i=0;i<vertexCount;i++) {
		b2Vec2 tmp = old_vertices[i];
		tmp *= mRatio;
        vertices[i] = Vec2(tmp.x, tmp.y);
	}
    
    Color4F fillColor = Color4F(color.r, color.g, color.b, 0.5);
    Color4F borderColor = Color4F(color.r, color.g, color.b, 1);
    
    drawNode->drawPolygon(vertices, vertexCount, fillColor, 1, borderColor);
    
    delete[] vertices;
}

void LHBox2dDebug::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    const float32 k_segments = 16.0f;
	int vertexCount=16;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	
	Vec2* vertices = new Vec2[vertexCount];
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
        
        vertices[i] = Vec2(v.x*mRatio, v.y*mRatio);
		theta += k_increment;
	}
	
    Color4F fillColor = Color4F(color.r, color.g, color.b, 0.5);
    Color4F borderColor = Color4F(color.r, color.g, color.b, 1);
    
    drawNode->drawPolygon(vertices, vertexCount, fillColor, 1, borderColor);
    
    delete[] vertices;
}

void LHBox2dDebug::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    const float32 k_segments = 16.0f;
	int vertexCount=16;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	
    Vec2* vertices = new Vec2[vertexCount];
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
        vertices[i] = Vec2(v.x*mRatio, v.y*mRatio);
		theta += k_increment;
	}
    
    Color4F fillColor = Color4F(color.r, color.g, color.b, 0.5);
    Color4F borderColor = Color4F(color.r, color.g, color.b, 1);
    
    drawNode->drawPolygon(vertices, vertexCount, fillColor, 1, borderColor);
    
    delete[] vertices;
    
	// Draw the axis line
	DrawSegment(center,center+radius*axis,color);
}

void LHBox2dDebug::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    Vec2 pointA  = Vec2(p1.x *mRatio,p1.y*mRatio);
    Vec2 pointB  = Vec2(p2.x*mRatio,p2.y*mRatio);
    
    Color4F borderColor = Color4F(color.r, color.g, color.b, 1);
    
    drawNode->drawSegment(pointA, pointB, 1, borderColor);
}

void LHBox2dDebug::DrawTransform(const b2Transform& xf)
{
    //	b2Vec2 p1 = xf.p, p2;
    //	const float32 k_axisScale = 0.4f;
    //
    //	p2 = p1 + k_axisScale * xf.q.col1;
    //	DrawSegment(p1,p2,b2Color(1,0,0));
    //
    //	p2 = p1 + k_axisScale * xf.q.col2;
    //	DrawSegment(p1,p2,b2Color(0,1,0));
}

void LHBox2dDebug::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
    //	glColor4f(color.r, color.g, color.b,1);
    //	glPointSize(size);
    //	GLfloat				glVertices[] = {
    //		p.x*mRatio,p.y*mRatio
    //	};
    //	glVertexPointer(2, GL_FLOAT, 0, glVertices);
    //	glDrawArrays(GL_POINTS, 0, 1);
    //	glPointSize(1.0f);
}

void LHBox2dDebug::DrawString(int x, int y, const char *string, ...)
{
	// Unsupported as yet. Could replace with bitmap font renderer at a later date
}

void LHBox2dDebug::DrawAABB(b2AABB* aabb, const b2Color& c)
{
    //	glColor4f(c.r, c.g, c.b,1);
    //
    //	GLfloat				glVertices[] = {
    //		aabb->lowerBound.x, aabb->lowerBound.y,
    //		aabb->upperBound.x, aabb->lowerBound.y,
    //		aabb->upperBound.x, aabb->upperBound.y,
    //		aabb->lowerBound.x, aabb->upperBound.y
    //	};
    //	glVertexPointer(2, GL_FLOAT, 0, glVertices);
    //	glDrawArrays(GL_LINE_LOOP, 0, 8);
	
}



LHBox2dDebugDrawNode::LHBox2dDebugDrawNode()
{
    _debug = nullptr;
    _drawState = true;
}

LHBox2dDebugDrawNode::~LHBox2dDebugDrawNode()
{
    if(_debug){
        delete _debug;
        _debug = nullptr;
    }
}

LHBox2dDebug* LHBox2dDebugDrawNode::getDebug()
{
    if(!_debug){
        _debug = new LHBox2dDebug(32, this);
    }
    return _debug;
}

LHBox2dDebugDrawNode* LHBox2dDebugDrawNode::create()
{
    LHBox2dDebugDrawNode *ret = new LHBox2dDebugDrawNode();
    if (ret && ret->init())
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

bool LHBox2dDebugDrawNode::init()
{
    if(DrawNode::init())
    {
        _physicsBody = NULL;
        return true;
    }
    return false;
}

#endif


