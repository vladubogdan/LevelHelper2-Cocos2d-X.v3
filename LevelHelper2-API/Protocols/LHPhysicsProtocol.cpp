//
//  LHPhysicsProtocol.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 24/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHPhysicsProtocol.h"
#include "LHNodeProtocol.h"

#include "LHDictionary.h"
#include "LHArray.h"

#include "LHConfig.h"
#include "LHUserProperties.h"
#include "LHValue.h"

#include "LHScene.h"
#include "LHSprite.h"
#include "LHShape.h"
#include "LHBezier.h"
#include "LHShape.h"
#include "LHRopeJointNode.h"
#include "LHParallax.h"
#include "LHParallaxLayer.h"
#include "LHWater.h"
#include "LHNode.h"
#include "LHAsset.h"
#include "LHCamera.h"


#include "LHGameWorldNode.h"
#include "LHUINode.h"
#include "LHBackUINode.h"
#include "LHDevice.h"

#if LH_USE_BOX2D
#include "Box2d/Box2d.h"
#endif

LHPhysicsProtocol::LHPhysicsProtocol()
{
#if LH_USE_BOX2D
    _body = NULL;
#endif
}
LHPhysicsProtocol::~LHPhysicsProtocol()
{
#if LH_USE_BOX2D
    this->removeBody();
#endif
}


Node* LHPhysicsProtocol::LHGetNode(LHPhysicsProtocol* prot)
{
    return dynamic_cast<Node*>(prot);
}



#pragma mark - BOX2D SUPPORT

#if LH_USE_BOX2D
void LHPhysicsProtocol::removeBody()
{
    if(_body){
        b2World* world = _body->GetWorld();
        if(world){
            if(!world->IsLocked()){
                world->DestroyBody(_body);
                _body = NULL;
                scheduledForRemoval = false;
            }
            else{
                scheduledForRemoval = true;
            }
        }
    }
}

void LHPhysicsProtocol::updatePhysicsTransform(){

    if(_body && scheduledForRemoval){
        this->removeBody();
    }
    
    if(_body)
    {
        Node* _node = dynamic_cast<Node*>(this);
        if(!_node)return;
        
        LHScene* scene = (LHScene*)_node->getScene();
        Point worldPos = _node->convertToWorldSpaceAR(Point());
        b2Vec2 b2Pos = scene->metersFromPoint(worldPos);
        _body->SetTransform(b2Pos, CC_DEGREES_TO_RADIANS(-_node->getRotation()));
    }
}

bool LHValidateCentroid(b2Vec2* vs, int count)
{
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
    return area > b2_epsilon;
    //	b2Assert(area > b2_epsilon);
}

void LHPhysicsProtocol::updatePhysicsScale(){

    if(_body){

        Node* _node = dynamic_cast<Node*>(this);
        if(!_node)return;

        float scaleX = _node->getScaleX();
        float scaleY = _node->getScaleY();
        
        if(scaleX < 0.01 && scaleX > -0.01){
            CCLOG("WARNING - SCALE Y value CANNOT BE 0 - BODY WILL NOT GET SCALED.");
            return;
        }
        
        if(scaleY < 0.01 && scaleY > -0.01){
            CCLOG("WARNING - SCALE X value CANNOT BE 0 - BODY WILL NOT GET SCALED.");
            return;
        }
        
        b2Fixture* fix = _body->GetFixtureList();
        while (fix) {
            
            b2Shape* shape = fix->GetShape();
            
            int flipx = scaleX < 0 ? -1 : 1;
            int flipy = scaleY < 0 ? -1 : 1;
            
            if(shape->GetType() == b2Shape::e_polygon)
            {
                b2PolygonShape* polShape = (b2PolygonShape*)shape;
                int32 count = polShape->GetVertexCount();
                
                b2Vec2* newVertices = new b2Vec2[count];
                
                for(int i = 0; i < count; ++i)
                {
                    const int idx = (flipx < 0 && flipy >= 0) || (flipx >= 0 && flipy < 0) ? count - i - 1 : i;
                    
                    b2Vec2 pt = polShape->GetVertex(i);
                    
                    if(scaleX - previousScale.x != 0)
                    {
                        pt.x /= previousScale.x;
                        pt.x *= scaleX;
                    }
                    
                    if(scaleY - previousScale.y)
                    {
                        pt.y /= previousScale.y;
                        pt.y *= scaleY;
                    }
                    
                    newVertices[idx] = pt;
                }
                
                bool valid = LHValidateCentroid(newVertices, count);
                if(!valid) {
                    //flip
                    b2Vec2* flippedVertices = new b2Vec2[count];
                    for(int i = 0; i < count; ++i)
                    {
                        flippedVertices[i] = newVertices[count - i - 1];
                    }
                    delete[] newVertices;
                    newVertices = flippedVertices;
                }
                
                polShape->Set(newVertices, count);
                delete[] newVertices;
            }
            
            if(shape->GetType() == b2Shape::e_circle)
            {
                b2CircleShape* circleShape = (b2CircleShape*)shape;
                float radius = circleShape->m_radius;
                
                float newRadius = radius/previousScale.x*scaleX;
                circleShape->m_radius = newRadius;
            }
            
            
            if(shape->GetType() == b2Shape::e_edge)
            {
                b2EdgeShape* edgeShape = (b2EdgeShape*)shape;
#pragma unused (edgeShape)
                CCLOG("EDGE SHAPE");
            }
            
            if(shape->GetType() == b2Shape::e_chain)
            {
                b2ChainShape* chainShape = (b2ChainShape*)shape;
                
                b2Vec2* vertices = chainShape->m_vertices;
                int32 count = chainShape->m_count;
                
                for(int i = 0; i < count; ++i)
                {
                    b2Vec2 pt = vertices[i];
                    b2Vec2 newPt = b2Vec2(pt.x/previousScale.x*scaleX, pt.y/previousScale.y*scaleY);
                    vertices[i] = newPt;
                }
            }
            
            
            fix = fix->GetNext();
        }
        
        previousScale = Point(scaleX, scaleY);
    }
}


static inline AffineTransform NodeToB2BodyTransform(Node *node)
{
	AffineTransform transform = AffineTransformIdentity;
	for(Node *n = node; n && !LHGameWorldNode::isLHGameWorldNode(n); n = n->getParent()){
		transform = AffineTransformConcat(transform, n->getNodeToParentAffineTransform());
	}
	return transform;
}

static inline AffineTransform b2BodyToParentTransform(Node *node, LHPhysicsProtocol *physicsImp)
{
	return AffineTransformConcat(physicsImp->absoluteTransform(), AffineTransformInvert(NodeToB2BodyTransform(node->getParent())));
}

AffineTransform LHPhysicsProtocol::absoluteTransform()
{
    AffineTransform transform = AffineTransformIdentity;

    Node* _node = dynamic_cast<Node*>(this);
    if(!_node)return transform;
    
    LHScene* scene  = (LHScene*)_node->getScene();
    b2Vec2 b2Pos    = _body->GetPosition();
    Point globalPos = scene->pointFromMeters(b2Pos);
    
    transform = AffineTransformTranslate(transform, globalPos.x, globalPos.y);
    transform = AffineTransformRotate(transform, _body->GetAngle());
    
    Size size = _node->getContentSize();
    transform = AffineTransformTranslate(transform, - size.width*0.5, - size.height*0.5);
	return transform;
}

void LHPhysicsProtocol::visitPhysicsProtocol()
{
    if(_body){

        Node* node = dynamic_cast<Node*>(this);
        if(!node)return;

        AffineTransform trans = b2BodyToParentTransform(node, this);
        Point localPos = PointApplyAffineTransform(node->getAnchorPointInPoints(), trans);
        
        float angle = CC_RADIANS_TO_DEGREES(-_body->GetAngle());
        
        this->updatePosition(localPos);
        this->updateRotation(angle);
    }
}

void setupFixtureWithInfo(b2FixtureDef* fixture, LHDictionary* fixInfo)
{
    fixture->density     = fixInfo->floatForKey("density");
    fixture->friction    = fixInfo->floatForKey("friction");
    fixture->restitution = fixInfo->floatForKey("restitution");
    fixture->isSensor    = fixInfo->boolForKey("sensor");
    
    fixture->filter.maskBits    = fixInfo->intForKey("mask");
    fixture->filter.categoryBits= fixInfo->intForKey("category");
}

void LHPhysicsProtocol::loadPhysicsFromDictionary(LHDictionary* dict, LHScene* scene)
{
    CCLOG("TRY TO CREATE PHYSICS for %p",this);
    if(!dict)return;
    
    Node* node = LH_GET_NODE_FROM_PHYSICS_PROTOCOL(this);
    if(!node)return;

    CCLOG("HAVE INFO for %p",this);

    
    int shapeType = dict->intForKey("shape");
    int type  = dict->intForKey("type");
    
    b2World* world = scene->getBox2dWorld();
    
    b2BodyDef bodyDef;
    bodyDef.type = (b2BodyType)type;
    
    Point position = node->convertToWorldSpaceAR(Point());
    
    b2Vec2 bodyPos = scene->metersFromPoint(position);
    
    bodyDef.position = bodyPos;
    
    float angle = node->getRotation();
    bodyDef.angle = CC_DEGREES_TO_RADIANS(-angle);
    bodyDef.userData = node;
    
    _body = world->CreateBody(&bodyDef);
    _body->SetUserData(node);
    
    _body->SetFixedRotation(dict->boolForKey("fixedRotation"));
    _body->SetGravityScale(dict->floatForKey("gravityScale"));
    
    _body->SetSleepingAllowed(dict->boolForKey("allowSleep"));
    _body->SetBullet(dict->boolForKey("bullet"));
    
    Size sizet = node->getContentSize();
    
    sizet.width  = scene->metersFromValue(sizet.width);
    sizet.height = scene->metersFromValue(sizet.height);
    
    float scaleX = node->getScaleX();
    float scaleY = node->getScaleY();
    
    previousScale = Point(scaleX, scaleY);
    
    
    sizet.width *= scaleX;
    sizet.height*= scaleY;
    
    
    LHDictionary* fixInfo = dict->dictForKey("genericFixture");
    LHArray* fixturesInfo = nullptr;
    
    CCLOG("SHAPE TYPE for %p %d", this, shapeType);
    
    if(shapeType == 0)//RECTANGLE
    {
        b2Shape* shape = new b2PolygonShape();
        ((b2PolygonShape*)shape)->SetAsBox(sizet.width*0.5f, sizet.height*0.5f);
        
        b2FixtureDef fixture;
        setupFixtureWithInfo(&fixture, fixInfo);
        
        fixture.shape = shape;
        _body->CreateFixture(&fixture);
        
        delete shape;
        shape = NULL;
        
        CCLOG("CREATE RECTANGLE FOR BODY for %p body %p", this, _body);
    }
    else if(shapeType == 1)//CIRCLE
    {
        b2Shape* shape = new b2CircleShape();
        ((b2CircleShape*)shape)->m_radius = sizet.width*0.5;
        
        b2FixtureDef fixture;
        setupFixtureWithInfo(&fixture, fixInfo);
        
        fixture.shape = shape;
        _body->CreateFixture(&fixture);
        
        delete shape;
        shape = NULL;
    }
    else if(shapeType == 2)//POLYGON
    {
        if(LHShape::isLHShape(node))
        {
            LHShape* shape = (LHShape*)node;
            
            std::vector<Point> triangles = shape->trianglePoints();
            
            for(int i = 0;  i < triangles.size(); i=i+3)
            {
                Point ptA = triangles[i];
                Point ptB = triangles[i+1];
                Point ptC = triangles[i+2];
                
                b2Vec2 *verts = new b2Vec2[3];
                
                verts[2] = scene->metersFromPoint(ptA);
                verts[1] = scene->metersFromPoint(ptB);
                verts[0] = scene->metersFromPoint(ptC);
                
                b2PolygonShape shapeDef;
                
                shapeDef.Set(verts, 3);
                
                b2FixtureDef fixture;
                
                setupFixtureWithInfo(&fixture, fixInfo);
                
                fixture.shape = &shapeDef;
                _body->CreateFixture(&fixture);
                delete[] verts;
                
            }
        }
    }
    else if(shapeType == 3)//CHAIN
    {
        if(LHBezier::isLHBezier(node))
        {
            LHBezier* bezier = (LHBezier*)node;
            
            std::vector<Point> points =  bezier->linePoints();
            
            std::vector< b2Vec2 > verts;
            
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
            }
            
            b2Shape* shape = new b2ChainShape();
            ((b2ChainShape*)shape)->CreateChain (&(verts.front()), (int)verts.size());
            
            b2FixtureDef fixture;
            setupFixtureWithInfo(&fixture, fixInfo);
            
            fixture.shape = shape;
            _body->CreateFixture(&fixture);
            
            delete shape;
            shape = NULL;
        }
        else if(LHShape::isLHShape(node))
        {
//            NSArray* points = [(LHShape*)_node outlinePoints];
//            
//            std::vector< b2Vec2 > verts;
//            
//            for(NSValue* val in points){
//                CGPoint pt = CGPointFromValue(val);
//                pt.x *= scaleX;
//                pt.y *= scaleY;
//                
//                verts.push_back([scene metersFromPoint:pt]);
//            }
//            
//            b2Shape* shape = new b2ChainShape();
//            ((b2ChainShape*)shape)->CreateChain (&(verts.front()), (int)verts.size());
//            
//            b2FixtureDef fixture;
//            [self setupFixture:&fixture withInfo:fixInfo];
//            
//            fixture.shape = shape;
//            _body->CreateFixture(&fixture);
//            
//            delete shape;
//            shape = NULL;
        }
        else{
            
            
        }
    }
    else if(shapeType == 4)//OVAL
    {
        fixturesInfo = dict->arrayForKey("ovalShape");
    }
    else if(shapeType == 5)//TRACED
    {
        std::string fixUUID = dict->stringForKey("fixtureUUID");
        fixturesInfo = (LHArray*)scene->tracedFixturesWithUUID(fixUUID);
        
    }
    
    if(fixturesInfo)
    {
        int flipx = scaleX < 0 ? -1 : 1;
        int flipy = scaleY < 0 ? -1 : 1;
        
        CCLOG("CREATE FIXUTRE FOR BODY for %p body %p", this, _body);
        
        for(int f = 0; f < fixturesInfo->count(); ++f)
        {
            LHArray* fixPoints = fixturesInfo->arrayAtIndex(f);

            int count = (int)fixPoints->count();
            if(count > 2)
            {
                b2Vec2 *verts = new b2Vec2[count];
                b2PolygonShape shapeDef;
                
                int i = 0;
                for(int j = count-1; j >=0; --j)
                {
                    const int idx = (flipx < 0 && flipy >= 0) || (flipx >= 0 && flipy < 0) ? count - i - 1 : i;
                    
                    Point point = fixPoints->pointAtIndex(j);

                    point.x *= scaleX;
                    point.y *= scaleY;
                    
                    point.y = -point.y;
                    
                    verts[idx] = scene->metersFromPoint(point);
                    ++i;
                }
                
                shapeDef.Set(verts, count);
                
                b2FixtureDef fixture;
                
                setupFixtureWithInfo(&fixture, fixInfo);
                
                fixture.shape = &shapeDef;
                _body->CreateFixture(&fixture);
                delete[] verts;
            }
        }
    }
    
    CCLOG("FINAL BODY for %p body %p", this, _body);
}

#else //chipmunk


#pragma mark - CHIPMUNK SUPPORT

void LHPhysicsProtocol::removeBody()
{
    
}

void LHPhysicsProtocol::updatePhysicsTransform(){
    //nothing to update on chipmunk - handled by cocos2d
}

void LHPhysicsProtocol::updatePhysicsScale(){
    //nothing to update on chipmunk - handled by cocos2d
}

void LHPhysicsProtocol::visitPhysicsProtocol()
{
    //nothing to update on chipmunk - update is handled by Cocos2d
}


void LHPhysicsProtocol::loadPhysicsFromDictionary(LHDictionary* dict, LHScene* scene)
{
    if(!dict)return;
    
    Node* node = LH_GET_NODE_FROM_PHYSICS_PROTOCOL(this);
    if(!node)return;
    
    int shape = dict->intForKey("shape");
    int type  = dict->intForKey("type");
    
    __Array* fixShapes = new __Array();
    fixShapes->init();
    
    __Array* fixturesInfo = NULL;
    
    if(shape == 0)//RECTANGLE
    {
        node->setPhysicsBody(PhysicsBody::createBox(node->getContentSize()));
    }
    else if(shape == 1)//CIRCLE
    {
        node->setPhysicsBody(PhysicsBody::createCircle(node->getContentSize().width*0.5));
    }
    else if(shape == 3)//CHAIN
    {
        LHBezier* bez = dynamic_cast<LHBezier*>(node);
        if(bez)
        {
            std::vector<Point> points = bez->linePoints();

            PhysicsBody* body = PhysicsBody::create();
            node->setPhysicsBody(body);

            Point *prevPt = nullptr;
            for(size_t i = 0; i < points.size(); ++i)
            {
                Point ptB = points[i];
                if(prevPt)
                {
                    PhysicsShapeEdgeSegment* shape = PhysicsShapeEdgeSegment::create(Point(prevPt->x, prevPt->y), ptB);
                    fixShapes->addObject(shape);
                    body->addShape(shape);
                }
                
                CC_SAFE_DELETE(prevPt);
                prevPt = new Point(ptB);
            }
            CC_SAFE_DELETE(prevPt);
        }
        else if(LHShape::isLHShape(node))
        {
            LHShape* shape = dynamic_cast<LHShape*>(node);
            
            std::vector<Point> points = shape->outlinePoints();
            
            PhysicsBody* body = PhysicsBody::create();
            node->setPhysicsBody(body);

            Point* firstPt = nullptr;
            Point* prevPt = nullptr;
            
            for(size_t i = 0; i < points.size(); ++i)
            {
                Point ptB = points[i];
            
                if(prevPt)
                {
                    PhysicsShapeEdgeSegment* shape = PhysicsShapeEdgeSegment::create(Point(prevPt->x, prevPt->y), ptB);
                    fixShapes->addObject(shape);
                    body->addShape(shape);
                }
                
                if(nullptr == firstPt){
                    firstPt = new Point(ptB);
                }
                CC_SAFE_DELETE(prevPt);
                prevPt = new Point(ptB);
            }
            
            //close the shape
            if(prevPt && firstPt){
                
                PhysicsShapeEdgeSegment* shape = PhysicsShapeEdgeSegment::create(Point(prevPt->x, prevPt->y), Point(firstPt->x, firstPt->y));
                fixShapes->addObject(shape);
                body->addShape(shape);
            }
            CC_SAFE_DELETE(prevPt);
            CC_SAFE_DELETE(firstPt);
        }
        else{
            type = 0;
            node->setPhysicsBody(PhysicsBody::createEdgeBox(node->getContentSize()));
        }
    }
    else if(shape == 4)//OVAL
    {
        fixturesInfo = dict->arrayForKey("ovalShape");
    }
    else if(shape == 5)//TRACED
    {
        if(dict->objectForKey("fixtureUUID"))
        {
            std::string fixUUID = dict->stringForKey("fixtureUUID");
            if(scene){
                fixturesInfo = scene->tracedFixturesWithUUID(fixUUID);
            }
        }
    }
    else if(shape == 2)//POLYGON
    {
        
        if(LHShape::isLHShape(node))
        {
            LHShape* shape = dynamic_cast<LHShape*>(node);
            std::vector<Point> trianglePoints = shape->trianglePoints();

            PhysicsBody* body = PhysicsBody::create();
            node->setPhysicsBody(body);

            for(size_t i = 0; i < trianglePoints.size(); i+=3)
            {
                
                Point ptA = trianglePoints[i];
                Point ptB = trianglePoints[i+1];
                Point ptC = trianglePoints[i+2];
                
                Point* points = new Point[3];
                
                points[0] = ptA;
                points[1] = ptB;
                points[2] = ptC;
                
                PhysicsShapePolygon* p_shape =  PhysicsShapePolygon::create(points, 3);
                body->addShape(p_shape);
                fixShapes->addObject(p_shape);
                
                delete[] points;
            }
        }
    }
    
    if(fixturesInfo)
    {
        PhysicsBody* body = PhysicsBody::create();
        node->setPhysicsBody(body);

        for(int f = 0; f < fixturesInfo->count(); ++f)
        {
            LHArray* fixPoints = (LHArray*)fixturesInfo->getObjectAtIndex(f);

            int count = (int)fixPoints->count();
            Point* points = new Point[count];
            
            int i = count - 1;
            for(int j = 0; j< count; ++j)
            {
                Point point = fixPoints->pointAtIndex(j);
                point.y = -point.y;
                
                points[j] = point;
                i = i-1;
            }
            
            PhysicsShapePolygon* shape = PhysicsShapePolygon::create(points, count);
            fixShapes->addObject(shape);
            body->addShape(shape);
            
            delete[] points;
        }
    }

    CC_SAFE_DELETE(fixShapes);
    
    if(node->getPhysicsBody())
    {
        if(type == 0)//static
        {
            node->getPhysicsBody()->setDynamic(false);
        }
        else if(type == 1)//kinematic
        {
        }
        else if(type == 2)//dynamic
        {
            node->getPhysicsBody()->setDynamic(true);
        }
    }
    
    LHDictionary* fixInfo = dict->dictForKey("genericFixture");
    if(fixInfo && node->getPhysicsBody())
    {
        int category = fixInfo->intForKey("category");
        int mask = fixInfo->intForKey("mask");
        
        node->getPhysicsBody()->setCollisionBitmask(mask);
        node->getPhysicsBody()->setCategoryBitmask(category);
        
        if(shape != 3)//chain
        {
//           Vector<PhysicsShape*> bodyShapes = node->getPhysicsBody()->getShapes();
//
//            for (Ref* obj : bodyShapes)
//            {
//                PhysicsShape* shp = dynamic_cast<PhysicsShape*>(obj);
//                
//                //setting density causes weird behaviour - WHY?
//                shp->setDensity(fixInfo->floatForKey("density"));
//                shp->setFriction(fixInfo->floatForKey("friction"));
//                shp->setRestitution(fixInfo->floatForKey("restitution"));
//
//                shp->setCollisionBitmask(mask);
//                shp->setCategoryBitmask(category);
//                
//            }
//                node->getPhysicsBody()->sensor = [fixInfo boolForKey:@"sensor"];
        }
        
        if(type == 2)//dynamic
        {
            node->getPhysicsBody()->setRotationEnable(!dict->boolForKey("fixedRotation"));
        }

        if(dict->intForKey("gravityScale") == 0){
            node->getPhysicsBody()->setGravityEnable(false);
        }
    }
}

#endif
