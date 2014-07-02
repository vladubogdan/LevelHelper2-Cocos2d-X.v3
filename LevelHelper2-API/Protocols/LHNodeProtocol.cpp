//
//  LHNodeProtocol.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 24/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHNodeProtocol.h"
#include "LHDictionary.h"
#include "LHArray.h"
#include "LHScene.h"
#include "LHUserProperties.h"
#include "LHShape.h"
#include "LHBezier.h"
#include "LHConfig.h"
#include "LHGameWorldNode.h"

#if LH_USE_BOX2D
#include "Box2d/Box2d.h"
#endif

LHNodeProtocol::LHNodeProtocol():name("Untitled"),userProperty(NULL)
{
//    printf("NODE PROTOCOL CONTSTRUCTOR\n");
    
#if LH_USE_BOX2D
    _body = nullptr;
#endif
    
}
LHNodeProtocol::~LHNodeProtocol()
{
//    printf("\nNODE PROTOCOL DEALLOC\n");
    
    if(userProperty){
        delete userProperty;
        userProperty = NULL;
    }
}

void LHNodeProtocol::setTags(LHArray* _tags_)
{
    tags.clear();
    for(int i = 0; i < _tags_->count(); ++i)
    {
        tags.push_back(_tags_->stringAtIndex(i));
    }
}

std::string LHNodeProtocol::getUuid()
{
    return uuid;
}
void LHNodeProtocol::setUuid(std::string value)
{
    uuid = std::string(value);
}


void LHNodeProtocol::loadUserPropertyWithDictionary(LHDictionary* dict)
{
    if(dict->objectForKey("userPropertyInfo"))
    {
        Node* node = dynamic_cast<Node*>(this);
        if(!node)return;
        
        LHDictionary* userPropInfo  = dict->dictForKey("userPropertyInfo");
        std::string userPropClassName = dict->stringForKey("userPropertyName");
        
        LHUserPropertyProtocol* propInstance = LHUserCustomPropertiesManager::customClassInstanceWithName(userPropClassName, node);
        
        if(propInstance)
        {
            propInstance->setPropertiesFromDictionary(userPropInfo);
            userProperty = propInstance;
        }
    }
}

Node* LHNodeProtocol::getChildNodeWithName(const std::string& name)
{
    Node* node = dynamic_cast<Node*>(this);
    if(!node)return NULL;
    
    auto& children = node->getChildren();
    for( const auto &n : children)
    {
        LHNodeProtocol* nProt = dynamic_cast<LHNodeProtocol*>(n);
        if(nProt)
        {
            if(nProt->getName() == name)
            {
                return n;
            }
            
            Node* retNode = nProt->getChildNodeWithName(name);
            if(retNode)
            {
                return retNode;
            }
        }
    }
    return NULL;
}

Node* LHNodeProtocol::getChildNodeWithUUID(const std::string& uuid)
{
    Node* node = dynamic_cast<Node*>(this);
    if(!node){
        return NULL;
    }
    
    auto& children = node->getChildren();
    for( const auto &n : children){
        LHNodeProtocol* nProt = dynamic_cast<LHNodeProtocol*>(n);
        if(nProt){
            if(nProt->getUuid() == uuid){
                return n;
            }
            
            Node* retNode = nProt->getChildNodeWithUUID(uuid);
            if(retNode){
                return retNode;
            }
        }
    }
    return NULL;
}

__Array* LHNodeProtocol::getChildrenWithTags(const std::vector<std::string>& tagValues, bool any)
{
    __Array* temp = __Array::create();
    
    Node* node = dynamic_cast<Node*>(this);
    if(!node){
        return NULL;
    }
    
    auto& children = node->getChildren();
    for( const auto &n : children){
        LHNodeProtocol* nProt = dynamic_cast<LHNodeProtocol*>(n);
        if(nProt){
            std::vector<std::string> childTags = nProt->getTags();
            
            int foundCount = 0;
            bool foundAtLeastOne = false;
            
            for(size_t i = 0; i < childTags.size(); ++i)
            {
                std::string tg = childTags[i];
                
                for(size_t j = 0; j < tagValues.size();++j)
                {
                    std::string st = tagValues[j];
                    if(st == tg)
                    {
                        ++foundCount;
                        foundAtLeastOne = true;
                        if(any){
                            break;
                        }
                    }
                }
                
                if(any && foundAtLeastOne){
                    temp->addObject(n);
                }
            }
            
            if(!any && foundAtLeastOne && foundCount == tagValues.size()){
                temp->addObject(n);
            }
            
            __Array* childArray = nProt->getChildrenWithTags(tagValues, any);
            if(childArray){
                temp->addObjectsFromArray(childArray);
            }
        }
    }
    return temp;
}

void LHNodeProtocol::loadGenericInfoFromDictionary(LHDictionary* dict){
    
    if(dict->objectForKey("name"))
        setName(dict->stringForKey("name"));
    
    setUuid(dict->stringForKey("uuid"));
    setTags(dict->arrayForKey("tags"));
    loadUserPropertyWithDictionary(dict);
}

#if LH_USE_BOX2D

#pragma mark - BOX2D SUPPORT
static inline AffineTransform NodeToB2BodyTransform(Node *node)
{
	AffineTransform transform = AffineTransformIdentity;
	for(Node *n = node; n && !LHGameWorldNode::isLHGameWorldNode(n); n = n->getParent()){
		transform = AffineTransformConcat(transform, n->getNodeToParentAffineTransform());
	}
	return transform;
}

static inline AffineTransform b2BodyToParentTransform(Node *node, LHNodeProtocol *physicsImp)
{
	return AffineTransformConcat(physicsImp->absoluteTransform(), AffineTransformInvert(NodeToB2BodyTransform(node->getParent())));
}

AffineTransform LHNodeProtocol::absoluteTransform()
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
    
//    if(!LHShape::isLHShape(_node) && !LHBezier::isLHBezier(_node))//whats going on here? Why?
        transform = AffineTransformTranslate(transform, - size.width*0.5, - size.height*0.5);
    
	return transform;
}

void LHNodeProtocol::visitNodeProtocol()
{
    if(_body){

        Node* node = dynamic_cast<Node*>(this);
        if(!node)return;

        AffineTransform trans = b2BodyToParentTransform(node, this);
        Point localPos = PointApplyAffineTransform(node->getAnchorPointInPoints(), trans);
        
        float angle = CC_RADIANS_TO_DEGREES(-_body->GetAngle());
        
        node->setPosition(localPos);
        node->setRotation(angle);
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

void LHNodeProtocol::loadPhysicsFromDictionary(LHDictionary* dict, LHScene* scene)
{
    if(!dict)return;
    
    Node* node = dynamic_cast<Node*>(this);
    if(!node)return;
    
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
                
//                ptA.y = -ptA.y;
//                ptB.y = -ptB.y;
//                ptC.y = -ptC.y;
                
//                ptA.x *= scaleX; ptA.y *= scaleY;
//                ptB.x *= scaleX; ptB.y *= scaleY;
//                ptC.x *= scaleX; ptC.y *= scaleY;
                
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
            
            for(int i = 0; i < points.size(); ++i)
            {
                Point pt = points[i];

                pt.x *= scaleX;
                pt.y *= scaleY;
                
                verts.push_back(scene->metersFromPoint(pt));
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
    
    
}

#else //chipmunk

void LHNodeProtocol::loadPhysicsFromDictionary(LHDictionary* dict, LHScene* scene)
{
    if(!dict)return;
    
    Node* node = dynamic_cast<Node*>(this);
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
                
                //setting density causes weird behaviour - WHY?
//                shp->setDensity(fixInfo->floatForKey("density"));
//                shp->setFriction(fixInfo->floatForKey("friction"));
//                shp->setRestitution(fixInfo->floatForKey("restitution"));

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
