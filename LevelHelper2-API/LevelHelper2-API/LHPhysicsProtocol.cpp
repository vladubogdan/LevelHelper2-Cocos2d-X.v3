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
#include "LHAsset.h"

#include "NodeTransform.h"

#if LH_USE_BOX2D
#include "Box2d/Box2d.h"
#endif

LHPhysicsProtocol::LHPhysicsProtocol()
{
#if LH_USE_BOX2D
    scheduledForRemoval = false;
    _body = NULL;
#endif
}
LHPhysicsProtocol::~LHPhysicsProtocol()
{

}

void LHPhysicsProtocol::shouldRemoveBody()
{
#if LH_USE_BOX2D
    LHNodeProtocol* nodeProt = dynamic_cast<LHNodeProtocol*>(this);
    if(_body && nodeProt && !nodeProt->isB2WorldDirty())
    {
        Node* node = dynamic_cast<Node*>(this);

        if(node){
            
            LHBox2dWorld* world = (LHBox2dWorld*)_body->GetWorld();
            if(world){
                LHScene* scene = (LHScene*)world->_scene;
                if(scene){
                    LHGameWorldNode* gw = scene->getGameWorldNode();
                    if(gw){
                        gw->removeScheduledContactsWithNode(node);
                    }
                }
            }
        }
        
        //do not remove the body if the scene is deallocing as the box2d world will be deleted
        //so we dont need to do this manualy
        //in some cases the nodes will be retained and removed after the box2d world is already deleted and we may have a crash
        this->removeBody();
    }
#endif
}

Node* LHPhysicsProtocol::LHGetNode(LHPhysicsProtocol* prot)
{
    return dynamic_cast<Node*>(prot);
}

LHAsset* LHPhysicsProtocol::assetParent()
{
    Node* p = LHPhysicsProtocol::LHGetNode(this);
    while(p && p->getParent()){
        if(LHAsset::isLHAsset(p))
            return (LHAsset*)p;
        p = p->getParent();
    }
    return nullptr;
}


#pragma mark - BOX2D SUPPORT

#if LH_USE_BOX2D

__Array* LHPhysicsProtocol::jointList(){
    
    __Array* array = __Array::create();
    
    if(_body != NULL){
        b2JointEdge* jtList = _body->GetJointList();
        while (jtList) {
            
            if(jtList->joint && jtList->joint->GetUserData())
            {
                Node* ourNode = (Node*)jtList->joint->GetUserData();
                if(ourNode != NULL)
                {
                    array->addObject(ourNode);
                }
            }
            jtList = jtList->next;
        }
    }
    return array;
}

bool LHPhysicsProtocol::removeAllAttachedJoints(){
    __Array* list = this->jointList();
    if(list)
    {
        for(size_t i = 0; i < list->count(); ++i)
        {
            Node* jt = (Node*)list->getObjectAtIndex(i);
            if(jt){
                jt->removeFromParent();
            }
        }
        return true;
    }
    return false;
}


void LHPhysicsProtocol::removeBody()
{
    if(_body){
        b2World* world = _body->GetWorld();
        if(world){

            _body->SetUserData(NULL);
            if(!world->IsLocked()){

                this->removeAllAttachedJoints();
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

    Node* _node = dynamic_cast<Node*>(this);
    if(!_node)return;

    if(_body)
    {
        LHScene* scene = (LHScene*)_node->getScene();
        LHGameWorldNode* gWNode = scene->getGameWorldNode();
        Point worldPos = _node->getParent()->convertToWorldSpace(_node->getPosition());
        worldPos = gWNode->convertToNodeSpace(worldPos);
        Point gWPos = gWNode->getPosition();
        worldPos = Point(worldPos.x - gWPos.x, worldPos.y - gWPos.y);
        
        b2Vec2 b2Pos = scene->metersFromPoint(worldPos);
        float globalAngle =  LHNodeTransform::globalAngleFromLocalAngle(_node, _node->getRotation());
        _body->SetTransform(b2Pos, CC_DEGREES_TO_RADIANS(-globalAngle));
        _body->SetAwake(true);
    }
}

bool LHValidateCentroid(b2Vec2* vs, int count)
{
	if(count < 3 || count > b2_maxPolygonVertices)
        return false;
    
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

void LHPhysicsProtocol::updatePhysicsScale(){

    if(_body){

        Node* _node = dynamic_cast<Node*>(this);
        if(!_node)return;

        float scaleX = _node->getScaleX();
        float scaleY = _node->getScaleY();
        
        Point globalScale = LHNodeTransform::convertToWorldScale(_node, Point(scaleX, scaleY));
        scaleX = globalScale.x;
        scaleY = globalScale.y;
        
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
	for(Node *n = node;
        n && !LHGameWorldNode::isLHGameWorldNode(n) && !LHUINode::isLHUINode(n)&& !LHBackUINode::isLHBackUINode(n);
        n = n->getParent()){
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
        float angle = LHNodeTransform::localAngleFromGlobalAngle(node, CC_RADIANS_TO_DEGREES(-_body->GetAngle()));
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
    if(!dict)return;
    
    Node* node = LH_GET_NODE_FROM_PHYSICS_PROTOCOL(this);
    if(!node)return;

    int shapeType = dict->intForKey("shape");
    int type  = dict->intForKey("type");
    
    b2World* world = scene->getBox2dWorld();
    
    b2BodyDef bodyDef;
    bodyDef.type = (b2BodyType)type;
    
//    Point position = node->convertToWorldSpaceAR(Point());
    Point position = node->getParent()->convertToWorldSpace(node->getPosition());
    b2Vec2 bodyPos = scene->metersFromPoint(position);
    bodyDef.position = bodyPos;
    
    float angle = LHNodeTransform::globalAngleFromLocalAngle(node, node->getRotation());
    bodyDef.angle = CC_DEGREES_TO_RADIANS(-angle);
    bodyDef.userData = node;
    
    _body = world->CreateBody(&bodyDef);
    _body->SetUserData(node);
    
    _body->SetFixedRotation(dict->boolForKey("fixedRotation"));
    _body->SetGravityScale(dict->floatForKey("gravityScale"));
    
    _body->SetSleepingAllowed(dict->boolForKey("allowSleep"));
    _body->SetBullet(dict->boolForKey("bullet"));
    
    if(dict->objectForKey("angularDamping"))//all this properties were added in the same moment
    {
        _body->SetAngularDamping(dict->floatForKey("angularDamping"));
    
        _body->SetAngularVelocity(dict->floatForKey("angularVelocity"));//radians/second.
    
        _body->SetLinearDamping(dict->floatForKey("linearDamping"));
        
        Point linearVel = dict->pointForKey("linearVelocity");
        _body->SetLinearVelocity(b2Vec2(linearVel.x,linearVel.y));
    }

    
    
    Size sizet = node->getContentSize();
    
    sizet.width  = scene->metersFromValue(sizet.width);
    sizet.height = scene->metersFromValue(sizet.height);
    
    float scaleX = node->getScaleX();
    float scaleY = node->getScaleY();
    
    Point worldScale = LHNodeTransform::convertToWorldScale(node, Point(scaleX, scaleY));
    scaleX = worldScale.x;
    scaleY = worldScale.y;
    
    previousScale = Point(scaleX, scaleY);
    
    
    sizet.width *= scaleX;
    sizet.height*= scaleY;
    
    
    LHDictionary* fixInfo = dict->dictForKey("genericFixture");
    LHArray* fixturesInfo = nullptr;
    
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
        if(!fixturesInfo){
            LHAsset* asset = this->assetParent();
            if(asset){
                fixturesInfo = (LHArray*)asset->tracedFixturesWithUUID(fixUUID);
            }
        }
    }
    
    if(fixturesInfo)
    {
        int flipx = scaleX < 0 ? -1 : 1;
        int flipy = scaleY < 0 ? -1 : 1;
        
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
                    
                    b2Vec2 vec = scene->metersFromPoint(point);

                    verts[idx] = vec;
                    ++i;
                }
                
                if(LHValidateCentroid(verts, count))
                {
                    shapeDef.Set(verts, count);
                    
                    b2FixtureDef fixture;
                    
                    setupFixtureWithInfo(&fixture, fixInfo);
                    
                    fixture.shape = &shapeDef;
                    _body->CreateFixture(&fixture);
                }

                delete[] verts;
            }
        }
    }
}

#else //chipmunk


#pragma mark - CHIPMUNK SUPPORT

void LHPhysicsProtocol::removeBody()
{
    Node* node = LH_GET_NODE_FROM_PHYSICS_PROTOCOL(this);
    if(!node)return;
    PhysicsBody* body = node->getPhysicsBody();
    if(body){
        body->removeFromWorld();
    }
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
    
    Size sizet = node->getContentSize();
    float scaleX = node->getScaleX();
    float scaleY = node->getScaleY();
    sizet.width *= scaleX;
    sizet.height*= scaleY;
    
    __Array* fixturesInfo = NULL;
    
    if(shape == 0)//RECTANGLE
    {
        node->setPhysicsBody(PhysicsBody::createBox(sizet));
    }
    else if(shape == 1)//CIRCLE
    {
        node->setPhysicsBody(PhysicsBody::createCircle(sizet.width*0.5));
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
                
                ptB.x *= scaleX;
                ptB.y *= scaleY;
                
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
            
                ptB.x *= scaleX;
                ptB.y *= scaleY;

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
            if(!fixturesInfo){
                LHAsset* asset = this->assetParent();
                if(asset){
                    fixturesInfo = (LHArray*)asset->tracedFixturesWithUUID(fixUUID);
                }
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
                
                ptA.x *= scaleX;
                ptA.y *= scaleY;

                ptB.x *= scaleX;
                ptB.y *= scaleY;

                ptC.x *= scaleX;
                ptC.y *= scaleY;
                
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

//        int flipx = scaleX < 0 ? -1 : 1;
//        int flipy = scaleY < 0 ? -1 : 1;

        
        for(int f = 0; f < fixturesInfo->count(); ++f)
        {
            LHArray* fixPoints = (LHArray*)fixturesInfo->getObjectAtIndex(f);

            int count = (int)fixPoints->count();
            if(count > 2)
            {
                Point* points = new Point[count];
                
//                int i = 0;
//                for(int j = count-1; j >=0; --j)
//                {
//                    const int idx = (flipx < 0 && flipy >= 0) || (flipx >= 0 && flipy < 0) ? count - i - 1 : i;
                
                int i = count - 1;
                for(int j = 0; j< count; ++j)
                {
                    
                    Point point = fixPoints->pointAtIndex(j);
                    point.y = -point.y;
                    
                    point.x *= scaleX;
                    point.y *= scaleY;
                    
                    points[j] = point;
//                    points[idx] = point;
                    i = i-1;
//                    ++i;
                }
                
                PhysicsShapePolygon* shape = PhysicsShapePolygon::create(points, count);
                fixShapes->addObject(shape);
                body->addShape(shape);
                
                delete[] points;
            }
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
        
        if(dict->objectForKey("angularDamping"))//all this properties were added in the same moment
        {
            node->getPhysicsBody()->setAngularDamping(dict->floatForKey("angularDamping"));
            node->getPhysicsBody()->setAngularVelocity(dict->floatForKey("angularVelocity"));
            node->getPhysicsBody()->setLinearDamping(dict->floatForKey("linearDamping"));
            Point linearVel = dict->pointForKey("linearVelocity");
            node->getPhysicsBody()->setVelocity(Vec2(linearVel.x, linearVel.y));
        }
    }
}

#endif
