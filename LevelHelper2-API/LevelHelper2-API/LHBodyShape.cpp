//
//  LHBodyShape.cpp
//  LevelHelper2-API
//
//  Created by Bogdan Vladu on 13/01/15.
//  Copyright (c) 2015 VLADU BOGDAN DANIEL PFA. All rights reserved.
//

#include "LHBodyShape.h"

#if LH_USE_BOX2D
#include "Box2d/Box2d.h"
#include "LHScene.h"
#include "LHValue.h"
#include "LHArray.h"
#include "LHDictionary.h"


bool LHBodyShape::LHValidateCentroid(b2Vec2* vs, int count)
{
    if(count > b2_maxPolygonVertices)
        return false;
    
    if(count < 3)
        return false;
    
    b2Vec2 c; c.Set(0.0f, 0.0f);
    float32 area = 0.0f;
    
    // pRef is the reference point for forming triangles.
    // It's location doesn't change the result (except for rounding error).
    b2Vec2 pRef(0.0f, 0.0f);
#if 0
    // This code would put the reference point inside the polygon.
    for (int32 i = 0; i < count; ++i)
    {
        pRef += vs[i];
    }
    pRef *= 1.0f / count;
#endif
    
    const float32 inv3 = 1.0f / 3.0f;
    
    for (int32 i = 0; i < count; ++i)
    {
        // Triangle vertices.
        b2Vec2 p1 = pRef;
        b2Vec2 p2 = vs[i];
        b2Vec2 p3 = i + 1 < count ? vs[i+1] : vs[0];
        
        b2Vec2 e1 = p2 - p1;
        b2Vec2 e2 = p3 - p1;
        
        float32 D = b2Cross(e1, e2);
        
        float32 triangleArea = 0.5f * D;
        area += triangleArea;
        
        // Area weighted centroid
        c += triangleArea * inv3 * (p1 + p2 + p3);
    }
    
    // Centroid
    if(area < b2_epsilon)
    {
        return false;
    }
    

	
	int32 n = b2Min(count, b2_maxPolygonVertices);
    
	// Perform welding and copy vertices into local buffer.
	b2Vec2 ps[b2_maxPolygonVertices];
	int32 tempCount = 0;
	for (int32 i = 0; i < n; ++i)
	{
		b2Vec2 v = vs[i];
        
		bool unique = true;
		for (int32 j = 0; j < tempCount; ++j)
		{
			if (b2DistanceSquared(v, ps[j]) < 0.5f * b2_linearSlop)
			{
				unique = false;
				break;
			}
		}
        
		if (unique)
		{
			ps[tempCount++] = v;
		}
	}
    
	n = tempCount;
	if (n < 3)
	{
		return false;
	}

    
    return true;
    
}


static void LHSetupb2FixtureWithInfo(b2FixtureDef* fixture, ValueMap& dict)
{
    fixture->density     = dict["density"].asFloat();
    fixture->friction    = dict["friction"].asFloat();
    fixture->restitution = dict["restitution"].asFloat();
    fixture->isSensor    = dict["sensor"].asBool();
    
    fixture->filter.maskBits    = dict["mask"].asInt();
    fixture->filter.categoryBits= dict["category"].asInt();
}

LHBodyShape::LHBodyShape()
{
}

LHBodyShape::~LHBodyShape()
{
}

LHBodyShape* LHBodyShape::createWithValueMap(ValueMap& dict, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY)
{
    LHBodyShape *ret = new LHBodyShape();
    if (ret && ret->initWithValueMap(dict, body, node, scene, scaleX, scaleY))
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

bool LHBodyShape::initWithValueMap(ValueMap& dict, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY)
{
    {
        shapeName   = dict["name"].asString();
        shapeID     = dict["shapeID"].asInt();
        
        int flipx = scaleX < 0 ? -1 : 1;
        int flipy = scaleY < 0 ? -1 : 1;
        
        Value fixturesPointsValue = dict["points"];
        
        
        float ratio = Director::getInstance()->getContentScaleFactor();
        
        
        if(!fixturesPointsValue.isNull())
        {
            ValueVector fixtures = fixturesPointsValue.asValueVector();
            
            for(int s = 0; s < fixtures.size(); ++s)
            {
                ValueVector fixPoints = fixtures[s].asValueVector();
                
                int count = (int)fixPoints.size();
                if(count > 2)
                {
                    
                    b2Vec2 *verts = new b2Vec2[count];
                    b2PolygonShape shapeDef;
                    
                    int i = 0;
                    for(int j = count-1; j >=0; --j)
                    {
                        const int idx = (flipx < 0 && flipy >= 0) || (flipx >= 0 && flipy < 0) ? count - i - 1 : i;
                        
                        std::string ptStr = fixPoints[j].asString();
                        Point point = PointFromString(ptStr);
                        
                        point.x /= ratio;
                        point.y /= ratio;
                        
                        point.x *= scaleX;
                        point.y *= scaleY;
                        
                        point.y = -point.y;
                        
                        b2Vec2 vec = scene->metersFromPoint(point);
                        
                        verts[idx] = vec;
                        ++i;
                    }
                    
                    if(LHValidateCentroid(verts, count))
                    {
                        shapeDef.Set(verts, count);
                        
                        b2FixtureDef fixture;
                        
                        LHSetupb2FixtureWithInfo(&fixture, dict);
                        
                        fixture.userData = this;
                        fixture.shape = &shapeDef;
                        body->CreateFixture(&fixture);
                    }
                    
                    delete[] verts;
                }
            }
        }
        else{
            
            float radius = dict["radius"].asFloat();
            std::string centerStr = dict["center"].asString();
            Point point = PointFromString(centerStr);
            
            radius /= ratio;
            
            point.x /= ratio;
            point.y /= ratio;
            
            point.x *= scaleX;
            point.y *= scaleY;
            
            point.y = -point.y;
            
            
            b2CircleShape* shape = new b2CircleShape();
            shape->m_radius = scene->metersFromValue(radius);
            shape->m_p = scene->metersFromPoint(point);
            
            b2FixtureDef fixture;
            LHSetupb2FixtureWithInfo(&fixture, dict);
            
            fixture.userData = this;
            fixture.shape = shape;
            body->CreateFixture(&fixture);
            
        }
        
        return true;
    }
    return false;
}

LHBodyShape* LHBodyShape::createCircleWithDictionary(ValueMap& dict, b2Body* body, Node* node, LHScene* scene, Size size)
{
    LHBodyShape *ret = new LHBodyShape();
    if (ret && ret->initCircleWithDictionary(dict, body, node, scene, size))
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

bool LHBodyShape::initCircleWithDictionary(ValueMap& dict, b2Body* body, Node* node, LHScene* scene, Size size)
{
    {
        shapeName   = dict["name"].asString();
        shapeID     = dict["shapeID"].asInt();
        
        b2CircleShape* shape = new b2CircleShape();
        shape->m_radius = size.width*0.5;
        
        b2FixtureDef fixture;
        LHSetupb2FixtureWithInfo(&fixture, dict);
        
        fixture.userData = this;
        fixture.shape = shape;
        body->CreateFixture(&fixture);
        
        delete shape;
        shape = NULL;
        
        return true;
    }
    return false;
}


LHBodyShape* LHBodyShape::createRectangleWithDictionary(ValueMap& dict, b2Body* body, Node* node, LHScene* scene, Size size)
{
    LHBodyShape *ret = new LHBodyShape();
    if (ret && ret->initRectangleWithDictionary(dict, body, node, scene, size))
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

bool LHBodyShape::initRectangleWithDictionary(ValueMap& dict, b2Body* body, Node* node, LHScene* scene, Size size)
{
    {
        shapeName   = dict["name"].asString();
        shapeID     = dict["shapeID"].asInt();
        
        b2PolygonShape* shape = new b2PolygonShape();
        shape->SetAsBox(size.width*0.5f, size.height*0.5f);
        
        b2FixtureDef fixture;
        LHSetupb2FixtureWithInfo(&fixture, dict);
        
        fixture.userData = this;
        fixture.shape = shape;
        body->CreateFixture(&fixture);
        
        delete shape;
        shape = NULL;

        return true;
    }
    return false;
}

LHBodyShape* LHBodyShape::createWithDictionary(ValueMap& dict, ValueVector& shapePoints, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY)
{
    LHBodyShape *ret = new LHBodyShape();
    if (ret && ret->initWithDictionary(dict, shapePoints, body, node, scene, scaleX, scaleY))
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

bool LHBodyShape::initWithDictionary(ValueMap& dict, ValueVector& shapePoints, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY)
{
    {
        shapeName   = dict["name"].asString();
        shapeID     = dict["shapeID"].asInt();
        
        int flipx = scaleX < 0 ? -1 : 1;
        int flipy = scaleY < 0 ? -1 : 1;
        
        
        for(int f = 0; f < shapePoints.size(); ++f)
        {
            Value fixPointsValue = shapePoints[f];;
            ValueVector fixPoints = fixPointsValue.asValueVector();
            
            int count = (int)fixPoints.size();
            if(count > 2)
            {
                b2Vec2 *verts = new b2Vec2[count];
                b2PolygonShape shapeDef;
                
                int i = 0;
                for(int j = count-1; j >=0; --j)
                {
                    const int idx = (flipx < 0 && flipy >= 0) || (flipx >= 0 && flipy < 0) ? count - i - 1 : i;
                    
                    std::string pointStr = fixPoints[j].asString();
                    Point point = PointFromString(pointStr);
                    
                    point.x *= scaleX;
                    point.y *= scaleY;
                    
                    point.y = -point.y;
                    
                    b2Vec2 vec = scene->metersFromPoint(point);
                    
                    verts[idx] = vec;
                    ++i;
                }
                
                if(LHValidateCentroid(verts, count))
                {
                    shapeDef.Set(verts, count);
                    
                    b2FixtureDef fixture;
                    
                    LHSetupb2FixtureWithInfo(&fixture, dict);
                    
                    fixture.userData = this;
                    fixture.shape = &shapeDef;
                    body->CreateFixture(&fixture);
                }
                
                delete[] verts;
            }
        }
        
        
        return true;
    }
    return false;
}



LHBodyShape* LHBodyShape::createWithName(const std::string& nm, const Point& ptA, const Point& ptB, Node* node, LHScene* scene)
{
    LHBodyShape *ret = new LHBodyShape();
    if (ret && ret->initWithName(nm, ptA, ptB, node, scene))
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

bool LHBodyShape::initWithName(const std::string& nm, const Point& ptA, const Point& ptB, Node* node, LHScene* scene)
{
    {
        shapeName   = nm;
        shapeID     = 0;
        
        // Define the ground body.
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0, 0); // bottom-left corner
        
        b2Body* physicsBoundariesBody = scene->getBox2dWorld()->CreateBody(&groundBodyDef);
        
        physicsBoundariesBody->SetUserData(node);
        
        // Define the ground box shape.
        b2EdgeShape groundBox;
        
        b2Vec2 from = scene->metersFromPoint(ptA);
        b2Vec2 to = scene->metersFromPoint(ptB);
        
        // top
        groundBox.Set(from, to);
        b2Fixture* fixture = physicsBoundariesBody->CreateFixture(&groundBox,0);
        fixture->SetUserData(this);
        
        return true;
    }
    return false;
}




LHBodyShape* LHBodyShape::createWithDictionaryAndTriangles(ValueMap& dict, const std::vector<Point>& triangles, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY)
{
    LHBodyShape *ret = new LHBodyShape();
    if (ret && ret->initWithDictionaryAndTriangles(dict, triangles, body, node, scene, scaleX, scaleY))
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

bool LHBodyShape::initWithDictionaryAndTriangles(ValueMap& dict, const std::vector<Point>& triangles, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY)
{
    {
        shapeName   = dict["name"].asString();
        shapeID     = dict["shapeID"].asInt();

        
        for(int i = 0;  i < triangles.size(); i=i+3)
        {
            Point ptA = triangles[i];
            Point ptB = triangles[i+1];
            Point ptC = triangles[i+2];
            
            ptA.x *= scaleX;
            ptA.y *= scaleY;
            
            ptB.x *= scaleX;
            ptB.y *= scaleY;
            
            ptC.x *= scaleX;
            ptC.y *= scaleY;
            
            b2Vec2 *verts = new b2Vec2[3];
            
            verts[2] = scene->metersFromPoint(ptA);
            verts[1] = scene->metersFromPoint(ptB);
            verts[0] = scene->metersFromPoint(ptC);
            
            b2PolygonShape shapeDef;
            
            shapeDef.Set(verts, 3);
            
            b2FixtureDef fixture;
            
            LHSetupb2FixtureWithInfo(&fixture, dict);
            
            fixture.userData = this;
            fixture.shape = &shapeDef;
            body->CreateFixture(&fixture);
            delete[] verts;
        }
        
        return true;
    }
    return false;
}

LHBodyShape* LHBodyShape::createChainWithDictionaryAndPoints(ValueMap& dict, const std::vector<Point>& points, bool close, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY)
{
    LHBodyShape *ret = new LHBodyShape();
    if (ret && ret->initChainWithDictionaryAndPoints(dict, points, close, body, node, scene, scaleX, scaleY))
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

bool LHBodyShape::initChainWithDictionaryAndPoints(ValueMap& dict, const std::vector<Point>& points, bool close, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY)
{
    {
        shapeName   = dict["name"].asString();
        shapeID     = dict["shapeID"].asInt();

        
        std::vector< b2Vec2 > verts;
        
        LHValue* firstPt = nullptr;
        LHValue* lastPt = nullptr;
        for(int i = 0; i < points.size(); ++i)
        {
            Point pt = points[i];
            
            pt.x *= scaleX;
            pt.y *= scaleY;
            
            b2Vec2 v2 = scene->metersFromPoint(pt);
            if(lastPt != nullptr)
            {
                Point oldPt = lastPt->getPoint();
                b2Vec2 v1 = b2Vec2(oldPt.x, oldPt.y);
                
                if(b2DistanceSquared(v1, v2) > b2_linearSlop * b2_linearSlop)
                {
                    verts.push_back(v2);
                }
            }
            else{
                verts.push_back(v2);
            }
            lastPt = LHValue::create(Point(v2.x, v2.y));
            
            if(firstPt == nullptr)
            {
                firstPt = LHValue::create(Point(v2.x, v2.y));
            }
        }
        
        if(firstPt && lastPt && close)
        {
            Point lastPoint = lastPt->getPoint();
            b2Vec2 v1 = b2Vec2(lastPoint.x, lastPoint.y);

            Point firstPoint = firstPt->getPoint();
            b2Vec2 v2 = b2Vec2(firstPoint.x, firstPoint.y);
            
            if(b2DistanceSquared(v1, v2) > b2_linearSlop * b2_linearSlop)
            {
                verts.push_back(v2);
            }
        }
        
        
        b2Shape* shape = new b2ChainShape();
        ((b2ChainShape*)shape)->CreateChain (&(verts.front()), (int)verts.size());
        
        b2FixtureDef fixture;

        LHSetupb2FixtureWithInfo(&fixture, dict);
        
        fixture.userData = this;
        fixture.shape = shape;
        body->CreateFixture(&fixture);
        
        delete shape;
        shape = NULL;
        
        return true;
    }
    return false;
}

bool LHBodyShape::isLHBodyShape(Ref* obj){
    return 0 != dynamic_cast<LHBodyShape*>(obj);
}

LHBodyShape* LHBodyShape::shapeForb2Fixture(b2Fixture* fix)
{
    if(0 == fix)
        return NULL;
    
    LHBodyShape* lhFix = (LHBodyShape*)fix->GetUserData();
    
    if(LHBodyShape::isLHBodyShape(lhFix)){
        return lhFix;
    }
    
    return NULL;
}

#endif
