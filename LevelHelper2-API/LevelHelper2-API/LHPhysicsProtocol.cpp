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

#include "LHUtils.h"

#if LH_USE_BOX2D
#include "Box2d/Box2d.h"
#include "LHBodyShape.h"
#endif

LHPhysicsProtocol::LHPhysicsProtocol()
{
#if LH_USE_BOX2D
    scheduledForRemoval = false;
    _body = NULL;
    subShapes = NULL;
#endif
}
LHPhysicsProtocol::~LHPhysicsProtocol()
{
#if LH_USE_BOX2D
    CC_SAFE_DELETE(subShapes);
    _body = NULL;
#endif
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

            CC_SAFE_DELETE(subShapes);
            
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
        Point worldPos = _node->getPosition();
        if(gWNode != _node->getParent()){
            worldPos = _node->getParent()->convertToWorldSpace(worldPos);
            worldPos = gWNode->convertToNodeSpace(worldPos);
        }
        
        b2Vec2 b2Pos = scene->metersFromPoint(worldPos);
        float globalAngle =  LHNodeTransform::globalAngleFromLocalAngle(_node, _node->getRotation());
        _body->SetTransform(b2Pos, CC_DEGREES_TO_RADIANS(-globalAngle));
        _body->SetAwake(true);
    }
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
                
                bool valid = LHBodyShape::LHValidateCentroid(newVertices, count);
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

void LHPhysicsProtocol::loadPhysicsFromDictionary(LHDictionary* dict, LHScene* scene)
{
    if(!dict)return;
    ValueMap map = LHUtils::Dictionary_To_ValueMap(dict);
    this->loadPhysicsFromValueMap(map, scene);
}

void LHPhysicsProtocol::loadPhysicsFromValueMap(ValueMap dict, LHScene* scene)
{
    Node* node = LH_GET_NODE_FROM_PHYSICS_PROTOCOL(this);
    if(!node)return;

    Value shapeTypeVal = dict["shape"];
    int shapeType = 6;//editor
    if(!shapeTypeVal.isNull())
        shapeType= shapeTypeVal.asInt();
    
    int type  = dict["type"].asInt();
    
    b2World* world = scene->getBox2dWorld();
    
    b2BodyDef bodyDef;
    bodyDef.type = (b2BodyType)type;
    
    Point position = node->getParent()->convertToWorldSpace(node->getPosition());
    b2Vec2 bodyPos = scene->metersFromPoint(position);
    bodyDef.position = bodyPos;
    
    float angle = LHNodeTransform::globalAngleFromLocalAngle(node, node->getRotation());
    bodyDef.angle = CC_DEGREES_TO_RADIANS(-angle);
    bodyDef.userData = node;
    
    _body = world->CreateBody(&bodyDef);
    _body->SetUserData(node);
    
    _body->SetFixedRotation(dict["fixedRotation"].asBool());
    _body->SetGravityScale(dict["gravityScale"].asFloat());
    
    _body->SetSleepingAllowed(dict["allowSleep"].asBool());
    _body->SetBullet(dict["bullet"].asBool());
    
    _body->SetAngularDamping(dict["angularDamping"].asFloat());
    _body->SetAngularVelocity(dict["angularVelocity"].asFloat());//radians/second.
    _body->SetLinearDamping(dict["linearDamping"].asFloat());
    
    std::string velStr = dict["linearVelocity"].asString();
    Point linearVel = PointFromString(velStr);
    _body->SetLinearVelocity(b2Vec2(linearVel.x,linearVel.y));
    
    
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
    
    Value genericFixVal = dict["genericFixture"];
    ValueMap fixInfo;
    if(!genericFixVal.isNull()){
        fixInfo = genericFixVal.asValueMap();
    }
    
    subShapes = new __Array();
    subShapes->init();
    
    if(shapeType == 0)//RECTANGLE
    {
        LHBodyShape* shape = LHBodyShape::createRectangleWithDictionary(fixInfo,
                                                                        _body,
                                                                        node,
                                                                        scene,
                                                                        sizet);
        if(shape){
            subShapes->addObject(shape);
        }
    }
    else if(shapeType == 1)//CIRCLE
    {
        LHBodyShape* shape = LHBodyShape::createCircleWithDictionary(fixInfo,
                                                                     _body,
                                                                     node,
                                                                     scene,
                                                                     sizet);
        if(shape){
            subShapes->addObject(shape);
        }
    }
    else if(shapeType == 2)//POLYGON
    {
        if(LHShape::isLHShape(node))
        {
            LHShape* shapeNode = (LHShape*)node;
            
            std::vector<Point> triangles = shapeNode->trianglePoints();
            
            LHBodyShape* shape = LHBodyShape::createWithDictionaryAndTriangles(fixInfo,
                                                                               triangles,
                                                                               _body,
                                                                               node,
                                                                               scene,
                                                                               scaleX,
                                                                               scaleY);
            if(shape){
                subShapes->addObject(shape);
            }
        }
    }
    else if(shapeType == 3)//CHAIN
    {
        if(LHBezier::isLHBezier(node))
        {
            LHBezier* bezierNode = (LHBezier*)node;
            
            std::vector<Point> points =  bezierNode->linePoints();
            
            LHBodyShape* shape = LHBodyShape::createChainWithDictionaryAndPoints(fixInfo, points, false, _body, node, scene, scaleX, scaleY);
            if(shape){
                subShapes->addObject(shape);
            }
            
        }
        else if(LHShape::isLHShape(node))
        {
            LHShape* nodeShape = (LHShape*)node;
            
            std::vector<Point> points = nodeShape->outlinePoints();
            
            LHBodyShape* shape = LHBodyShape::createChainWithDictionaryAndPoints(fixInfo, points, true, _body, node, scene, scaleX, scaleY);
            if(shape){
                subShapes->addObject(shape);
            }
        }
        else{
            
            
        }
    }
    else if(shapeType == 4)//OVAL
    {
        Value fixturesInfoValue = dict["ovalShape"];
        if(!fixturesInfoValue.isNull())
        {
            ValueVector fixturesInfo =fixturesInfoValue.asValueVector();
        
            LHBodyShape* shape = LHBodyShape::createWithDictionary(fixInfo,
                                                                   fixturesInfo,
                                                                   _body,
                                                                   node,
                                                                   scene,
                                                                   scaleX,
                                                                   scaleY);
            if(shape){
                subShapes->addObject(shape);
            }
        }

    }
    else if(shapeType == 5)//TRACED
    {
        std::string fixUUID = dict["fixtureUUID"].asString();
        LHArray* fixturesInfo = (LHArray*)scene->tracedFixturesWithUUID(fixUUID);
        if(!fixturesInfo){
            LHAsset* asset = this->assetParent();
            if(asset){
                fixturesInfo = (LHArray*)asset->tracedFixturesWithUUID(fixUUID);
            }
        }
        
        if(fixturesInfo){
            
            ValueVector fixVector = LHUtils::Array_To_ValueVector(fixturesInfo);
            LHBodyShape* shape = LHBodyShape::createWithDictionary(fixInfo,
                                                                   fixVector,
                                                                   _body,
                                                                   node,
                                                                   scene,
                                                                   scaleX,
                                                                   scaleY);
            if(shape){
                subShapes->addObject(shape);
            }
        }
    }
    else if(shapeType == 6)//editor
    {
        //available only on sprites
        LHSprite* sprite = dynamic_cast<LHSprite*>(node);
        
        if(!sprite)return;
        
        Value* bodyInfo = scene->getEditorBodyInfoForSpriteName(sprite->getSpriteFrameName(), sprite->getImageFilePath());
        
        ValueMap info = bodyInfo->asValueMap();
        ValueVector fixturesInfo = info["shapes"].asValueVector();
        
        for(int f = 0; f < fixturesInfo.size(); ++f)
        {
            ValueMap shapeInfo = fixturesInfo[f].asValueMap();
            
            LHBodyShape* shape = LHBodyShape::createWithValueMap(shapeInfo,
                                                                 _body,
                                                                 node,
                                                                 scene,
                                                                 scaleX,
                                                                 scaleY);
            if(shape){
                subShapes->addObject(shape);
            }
        }
        
        return;
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

void LHPhysicsProtocol::loadPhysicsFromValueMap(ValueMap value_Map, LHScene* scene){
    //not used by chipmunk yet
}

static bool LHValidateChipmunkCentroid(Point* vs, int count)
{
    const int maxPolygonVertices = 8;
    
    if(count > maxPolygonVertices)
        return false;
    
    if(count < 3)
        return false;
    
    Point c(0.0f, 0.0f);
    float area = 0.0f;
    
    // pRef is the reference point for forming triangles.
    // It's location doesn't change the result (except for rounding error).
    Point pRef(0.0f, 0.0f);

    const float inv3 = 1.0f / 3.0f;
    
    for (int i = 0; i < count; ++i)
    {
        // Triangle vertices.
        Point p1 = pRef;
        Point p2 = vs[i];
        Point p3 = i + 1 < count ? vs[i+1] : vs[0];
        
        Point e1 = p2 - p1;
        Point e2 = p3 - p1;
        
        float D = e1.x * e2.y - e1.y * e2.x;//cross
        
        float triangleArea = 0.5f * D;
        area += triangleArea;
        
        // Area weighted centroid
        c += triangleArea * inv3 * (p1 + p2 + p3);
    }
    
    // Centroid
    if(area < FLT_EPSILON)
    {
        return false;
    }
    
    const float linearSlop = 0.005f;
    
    int n = MIN(count, maxPolygonVertices);
    
    // Perform welding and copy vertices into local buffer.
    Point ps[maxPolygonVertices];
    int tempCount = 0;
    for (int i = 0; i < n; ++i)
    {
        Point v = vs[i];
        
        bool unique = true;
        for (int j = 0; j < tempCount; ++j)
        {
            
            Point distC(v.x - ps[j].x, v.y - ps[j].y);
            float dot = distC.x * distC.x + distC.y * distC.y;
            
            if (dot < 0.5f * linearSlop)
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
        //fixturesInfo = dict->arrayForKey("ovalShape");
    }
    else if(shape == 5)//TRACED
    {
//        if(dict->objectForKey("fixtureUUID"))
//        {
//            std::string fixUUID = dict->stringForKey("fixtureUUID");
//            if(scene){
//                fixturesInfo = scene->tracedFixturesWithUUID(fixUUID);
//            }
//            if(!fixturesInfo){
//                LHAsset* asset = this->assetParent();
//                if(asset){
//                    fixturesInfo = (LHArray*)asset->tracedFixturesWithUUID(fixUUID);
//                }
//            }
//        }
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

        int flipx = scaleX < 0 ? -1 : 1;
        int flipy = scaleY < 0 ? -1 : 1;

        
//        for(int f = 0; f < fixturesInfo->count(); ++f)
//        {
//            Value fixPointsValue = shapePoints[f];;
//            ValueVector fixPoints = fixPointsValue.asValueVector();
//            
//            int count = (int)fixPoints.size();
//            if(count > 2)
//            {
//                b2Vec2 *verts = new b2Vec2[count];
//                b2PolygonShape shapeDef;
//                
//                int i = 0;
//                for(int j = count-1; j >=0; --j)
//                {
//                    const int idx = (flipx < 0 && flipy >= 0) || (flipx >= 0 && flipy < 0) ? count - i - 1 : i;
//
//                    std::string pointStr = fixPoints[j].asString();
//                    Point point = PointFromString(pointStr);
//                    
//                    point.x *= scaleX;
//                    point.y *= scaleY;
//                    
//                    point.y = -point.y;
//                    
//                    b2Vec2 vec = scene->metersFromPoint(point);
//                    
//                    verts[idx] = vec;
//                    ++i;
//                }
//                
//                if(LHValidateCentroid(verts, count))
//                {
//                    shapeDef.Set(verts, count);
//                    
//                    b2FixtureDef fixture;
//                    
//                    LHSetupb2FixtureWithInfo(&fixture, dict);
//                    
//                    fixture.userData = this;
//                    fixture.shape = &shapeDef;
//                    body->CreateFixture(&fixture);
//                }
//                
//                delete[] verts;
//            }
//        }
        

        
        
        
        printf("SHAPE.......................\n");
        for(int f = 0; f < fixturesInfo->count(); ++f)
        {
            LHArray* fixPoints = (LHArray*)fixturesInfo->getObjectAtIndex(f);

            printf("FIXTURE.........\n");
            
            int count = (int)fixPoints->count();
            if(count > 2)
            {
                Point* points = new Point[count];
                
                int i = 0;
                for(int j = count-1; j >=0; --j)
                {
                    const int idx = (flipx < 0 && flipy >= 0) || (flipx >= 0 && flipy < 0) ? count - i - 1 : i;

                
//                int i = count - 1;
//                for(int j = 0; j< count; ++j)
//                {
                    
                    Point point = fixPoints->pointAtIndex(j);
                    
                    point.x *= scaleX;
                    point.y *= scaleY;

                    point.y = -point.y;

                    printf("PT %f %f\n", point.x, point.y);
//                    points[j] = point;
                    points[idx] = point;
//                    i = i-1;
                    ++i;
                }
                

                if(LHValidateChipmunkCentroid(points, count))
                {
                    
                    PhysicsShapePolygon* shape = PhysicsShapePolygon::create(points, count);
                    fixShapes->addObject(shape);
                    body->addShape(shape);
                }
                
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
