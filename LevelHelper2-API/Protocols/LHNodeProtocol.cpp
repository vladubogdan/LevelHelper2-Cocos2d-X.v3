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

LHNodeProtocol::LHNodeProtocol():name("Untitled"),userProperty(NULL)
{
//    printf("NODE PROTOCOL CONTSTRUCTOR\n");
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
    if(!node)return NULL;
    
    auto& children = node->getChildren();
    for( const auto &n : children)
    {
        LHNodeProtocol* nProt = dynamic_cast<LHNodeProtocol*>(n);
        if(nProt)
        {
            if(nProt->getUuid() == uuid)
            {
                return n;
            }
            
            Node* retNode = nProt->getChildNodeWithUUID(uuid);
            if(retNode)
            {
                return retNode;
            }
        }
    }
    return NULL;
}
void LHNodeProtocol::loadGenericInfoFromDictionary(LHDictionary* dict){
    
    if(dict->objectForKey("name"))
        setName(dict->stringForKey("name"));
    
    setUuid(dict->stringForKey("uuid"));
    setTags(dict->arrayForKey("tags"));
    loadUserPropertyWithDictionary(dict);
}

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
           Vector<PhysicsShape*> bodyShapes = node->getPhysicsBody()->getShapes();
            
            for (Ref* obj : bodyShapes)
            {
                PhysicsShape* shp = dynamic_cast<PhysicsShape*>(obj);
                
                //setting density causes weird behaviour - WHY?
//                shp->setDensity(fixInfo->floatForKey("density"));
//                shp->setFriction(fixInfo->floatForKey("friction"));
//                shp->setRestitution(fixInfo->floatForKey("restitution"));

//                shp->setCollisionBitmask(mask);
//                shp->setCategoryBitmask(category);
                
            }
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
