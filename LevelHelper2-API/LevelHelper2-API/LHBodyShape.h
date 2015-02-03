//
//  LHBodyShape.h
//  LevelHelper2-API
//
//  Created by Bogdan Vladu on 13/01/15.
//  Copyright (c) 2015 VLADU BOGDAN DANIEL PFA. All rights reserved.
//

#ifndef __LEVELHELPER_API_PHYSICS_BODY_SHAPE_H__
#define __LEVELHELPER_API_PHYSICS_BODY_SHAPE_H__

#if LH_USE_BOX2D

#include "cocos2d.h"
#include "LHConfig.h"

class b2Fixture;
class b2Body;
class b2Vec2;

class LHScene;
class LHDictionary;
class LHArray;

using namespace cocos2d;

class LHBodyShape : public Ref
{
    
public:
    
    static LHBodyShape* createWithValueMap(ValueMap& dict, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY);
    virtual bool initWithValueMap(ValueMap& dict, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY);
    
    static LHBodyShape* createCircleWithDictionary(ValueMap& dict, b2Body* body, Node* node, LHScene* scene,  Size size);
    virtual bool initCircleWithDictionary(ValueMap& dict, b2Body* body, Node* node, LHScene* scene,  Size size);
    
    static LHBodyShape* createRectangleWithDictionary(ValueMap& dict, b2Body* body, Node* node, LHScene* scene,  Size size);
    virtual bool initRectangleWithDictionary(ValueMap& dict, b2Body* body, Node* node, LHScene* scene,  Size size);
    
    static LHBodyShape* createWithDictionary(ValueMap& dict, ValueVector& shapePoints, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY);
    virtual bool initWithDictionary(ValueMap& dict, ValueVector& shapePoints, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY);
    
    static LHBodyShape* createWithDictionaryAndTriangles(ValueMap& dict, const std::vector<Point>& triangles, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY);
    virtual bool initWithDictionaryAndTriangles(ValueMap& dict, const std::vector<Point>& triangles, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY);
    
    static LHBodyShape* createWithName(const std::string& nm, const Point& ptA, const Point& ptB, Node* node, LHScene* scene);
    virtual bool initWithName(const std::string& nm, const Point& ptA, const Point& ptB, Node* node, LHScene* scene);
    
    static LHBodyShape* createChainWithDictionaryAndPoints(ValueMap& dict, const std::vector<Point>& points, bool close, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY);
    virtual bool initChainWithDictionaryAndPoints(ValueMap& dict, const std::vector<Point>& points, bool close, b2Body* body, Node* node, LHScene* scene, float scaleX, float scaleY);
    
    
    static bool LHValidateCentroid(b2Vec2* vs, int count);
    
    LHBodyShape();
    virtual ~LHBodyShape();
    
    const std::string& getShapeName(){return shapeName;}
    const int getShapeID(){return shapeID;}
    
    static bool isLHBodyShape(Ref* obj);
    static LHBodyShape* shapeForb2Fixture(b2Fixture* fix);
    
private:
    std::string shapeName;
    int shapeID;
};


#endif


#endif /* defined(__LEVELHELPER_API_PHYSICS_BODY_SHAPE_H__) */
