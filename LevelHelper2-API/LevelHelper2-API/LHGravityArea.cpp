//
//  LHGravityArea.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHGravityArea.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHDevice.h"
#include "LHGameWorldNode.h"


LHGravityArea::LHGravityArea()
{
}

LHGravityArea::~LHGravityArea()
{
    
}

LHGravityArea* LHGravityArea::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHGravityArea *ret = new LHGravityArea();
    if (ret && ret->initWithDictionary(dict, prnt))
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

bool LHGravityArea::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        this->loadGenericInfoFromDictionary(dict);
        
        prnt->addChild(this);
        this->loadTransformationInfoFromDictionary(dict);

        
        //we reset the scale back to 1 because the NodeProtocolImpl is setting the scale to wrong value
        this->setScaleX(1);
        this->setScaleY(1);
        
        Point scl = dict->pointForKey("scale");
        Size size = dict->sizeForKey("size");
        size.width *= scl.x;
        size.height *= scl.y;
        this->setContentSize(size);

        
        _direction  = dict->pointForKey("direction");
        _force      = dict->floatForKey("force");
        _radial     = dict->intForKey("type") == 1;
        
        
#if LH_DEBUG
        DrawNode* debug = DrawNode::create();
        this->addChild(debug);
        
        
        if(_radial)
        {
            const float k_segments = 32.0f;
            int vertexCount=32;
            const float k_increment = 2.0f * M_PI / k_segments;
            float theta = 0.0f;
            
            Point* vertices = new Point[vertexCount];
            for (int i = 0; i < k_segments; ++i){
                vertices[i] = Point(size.width*0.5 *cosf(theta), size.width*0.5 *sinf(theta));
                theta += k_increment;
            }
            
            Color4F borderColor = Color4F(0, 0, 1, 1);
            Color4F fillColor = Color4F(0,0,1,0.3);

            debug->drawPolygon(vertices, vertexCount, fillColor, 1, borderColor);
            
            delete[] vertices;
        }
        else{
            
            Point* vertices = new Point[4];
            
            vertices[0] = Point(-size.width*0.5, -size.height*0.5);
            vertices[1] = Point(size.width*0.5, -size.height*0.5);
            vertices[2] = Point(size.width*0.5, size.height*0.5);
            vertices[3] = Point(-size.width*0.5, size.height*0.5);
            
            Color4F borderColor = Color4F(0, 1, 0, 1);
            Color4F fillColor = Color4F(0,1,0,0.3);

            debug->drawPolygon(vertices, 4, fillColor, 1, borderColor);
            
            delete[] vertices;
        }
#endif//LH_DEBUG

        
        return true;
    }
    return false;
}

bool LHGravityArea::isRadial(){
    return _radial;
}

Point LHGravityArea::getDirection(){
    return _direction;
}

float LHGravityArea::getForce(){
    return _force;
}

Rect LHGravityArea::globalRect(){
    
    Point pos = this->convertToWorldSpaceAR(Point());
    
    Size size = this->getContentSize();
    return Rect(pos.x - size.width*0.5,
                pos.y - size.height*0.5,
                size.width,
                size.height);
}

#if LH_USE_BOX2D
#if COCOS2D_VERSION >= 0x00030200
void LHGravityArea::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
#else
void LHGravityArea::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
#endif
{
    LHScene* scene = (LHScene*)this->getScene();
    LHGameWorldNode* pNode = scene->getGameWorldNode();

    b2World* world =  pNode->getBox2dWorld();
    
    if(!world)return;
    
    Size size = this->getContentSize();
    float ptm = scene->getPtm();
    Rect rect = this->globalRect();
    
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
        if(this->isRadial())
        {
            Point globalPos = this->convertToWorldSpaceAR(Point());
            
            b2Vec2 b2TouchPosition = scene->metersFromPoint(globalPos);
            b2Vec2 b2BodyPosition = b2Vec2(b->GetPosition().x, b->GetPosition().y);
    
            float maxDistance = scene->metersFromValue(size.width*0.5);
            float maxForce = -this->getForce()/ptm;
            
            float distance = b2Distance(b2BodyPosition, b2TouchPosition);
            if(distance < maxDistance)
            {
                float strength = (maxDistance - distance) / maxDistance;
                float force = strength * maxForce;
                float angle = atan2f(b2BodyPosition.y - b2TouchPosition.y, b2BodyPosition.x - b2TouchPosition.x);
                
                #ifdef B2_MOTOR_JOINT_H
                    //2.3.x
                    b->ApplyLinearImpulse(b2Vec2(cosf(angle) * force, sinf(angle) * force), b->GetPosition(), true);
                #else
                    //2.2.1
                    b->ApplyLinearImpulse(b2Vec2(cosf(angle) * force, sinf(angle) * force), b->GetPosition());
                #endif
            }
        }
        else{
            b2Vec2 b2BodyPosition = b2Vec2(b->GetPosition().x, b->GetPosition().y);
            
            Point pos = scene->pointFromMeters(b2BodyPosition);
            
            if(rect.containsPoint(pos))
            {
                float force = this->getForce()/ptm;

                #ifdef B2_MOTOR_JOINT_H
                //2.3.x
                b->ApplyLinearImpulse(b2Vec2(_direction.x * force, _direction.y * force), b->GetPosition(), true);
                #else
                //2.2.1
                b->ApplyLinearImpulse(b2Vec2(_direction.x * force, _direction.y * force), b->GetPosition());
                #endif
            }
        }
	}
    
    if(renderer)
    {
#if COCOS2D_VERSION >= 0x00030200
        Node::visit(renderer, parentTransform, parentFlags);
#else
        Node::visit(renderer, parentTransform, parentTransformUpdated);
#endif
    }
}

#else //chipmunk


bool LHGravityAreaPhysicsQuery(PhysicsWorld& world, PhysicsShape& shape, void* info)
{
    LHGravityArea* area = (LHGravityArea*)info;
    
    PhysicsBody* body = shape.getBody();
    if(body && body->isDynamic())
    {
        Node* node = body->getNode();
        Point pos = node->convertToWorldSpaceAR(Point());

        if(area->isRadial())
        {
            Point position = area->getPosition();
            
            float maxDistance = area->getContentSize().width*0.5f;
            float distance = position.distance(pos);//LHDistanceBetweenPoints(position, pos);
            
            if(distance < maxDistance)
            {
                float maxForce = -area->getForce();
                float strength = (maxDistance - distance) / maxDistance;
                float force = strength * maxForce;
                float angle = atan2f(pos.y - position.y, pos.x - position.x);
                
                body->applyImpulse(Point(cosf(angle) * force,
                                         sinf(angle) * force));
            }
        }
        else{
            float force = area->getForce();
            Point direction = area->getDirection();
            Point pt = Point(direction.x * force, direction.y * force);
            body->applyImpulse(pt);
        }
    }

    
    
    return  true;//continue
}

#if COCOS2D_VERSION >= 0x00030200
void LHGravityArea::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
#else
void LHGravityArea::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
#endif
{
    Rect rect = this->globalRect();
    
    PhysicsWorld* world = this->getScene()->getPhysicsWorld();
    
    world->queryRect(&LHGravityAreaPhysicsQuery, rect, this);
    
    if(renderer)
    {
#if COCOS2D_VERSION >= 0x00030200
        Node::visit(renderer, parentTransform, parentFlags);
#else
        Node::visit(renderer, parentTransform, parentTransformUpdated);
#endif
    }
}
#endif

