//
//  LHRopeJointNode.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 27/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHRopeJointNode.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHDevice.h"
#include "LHUtils.h"
#include "LHDrawNode.h"
#include "LHValue.h"
#include "LHGameWorldNode.h"
#include "LHPhysicsProtocol.h"


#if LH_USE_BOX2D
#include "Box2d/Box2d.h"
#endif

//we implement this function because its missing in Windows Phone 8 c++ lib.
double asinh(double x)
{
    return log(x + sqrt(x*x + 1.0));
}

double bisection(double g0, double g1, double epsilon,
                 double (*fp)(double, void *), void *data)
{
    if(!data)return 0;
    
    double v0, g, v;
    v0 = fp(g0, data);
    
    while(fabs(g1-g0) > fabs(epsilon)){
        g = (g0+g1)/2.0;
        v = fp(g, data);
        if(v == 0.0)
            return g;
        else if(v*v0 < 0.0){
            g1 = g;
        } else {
            g0 = g;   v0 = v;
        }
    }
    
    return (g0+g1)/2.0;
}

double f(double x, void *data)
{
    if(!data)return 0;
    double *input = (double *)data;
    double secondTerm, delX, delY, L;
    delX  = input[2] - input[0];
    delY  = input[3] - input[1];
    L     = input[4];
    secondTerm = sqrt(L*L - delY*delY)/delX;
    
    return (sinh(x)/x -secondTerm);
}

/* f(x) = y0 + A*(cosh((x-x0)/A) - 1) */
double fcat(double x, void *data)
{
    if(!data)return 0;
    
    double x0, y0, A;
    double *input = (double *)data;
    x0  = input[0];
    y0  = input[1];
    A   = input[2];
    
    return y0 + A*(cosh((x-x0)/A) - 1.0);
}


LHRopeJointNode::LHRopeJointNode()
{
    cutJointA = nullptr;
    cutJointB = nullptr;
#if LH_USE_BOX2D
    cutJointAB= nullptr;
#endif
    
    ropeShape = nullptr;
    cutAShapeNode = nullptr;
    cutBShapeNode = nullptr;
    _cutTimer = -1;
}

void LHRopeJointNode::shouldRemoveJoint()
{
#if LH_USE_BOX2D
    LHNodeProtocol* nodeProt = dynamic_cast<LHNodeProtocol*>(this);
    if(cutBodyA && nodeProt && !nodeProt->isB2WorldDirty())
    {
        b2World* world = cutBodyA->GetWorld();
        if(cutBodyA){
            world->DestroyBody(cutBodyA);
        }
        cutBodyA = nullptr;
        cutJointA = nullptr;
        
        if(cutBodyB){
            world->DestroyBody(cutBodyB);
        }
        cutBodyB = nullptr;
        cutJointB = nullptr;
        
        if(cutJointAB){
            world->DestroyJoint(cutJointAB);
        }
        cutJointAB = nullptr;
    }
#else
    cutJointA->removeFormWorld();
    cutJointB->removeFormWorld();
    cutJointA = nullptr;
    cutJointB = nullptr;    
#endif
    
    LHJointsProtocol::shouldRemoveJoint();
}


LHRopeJointNode::~LHRopeJointNode()
{
    this->shouldRemoveJoint();
    
    ropeShape = nullptr;

    cutAShapeNode = nullptr;
    cutJointA = nullptr;
    
    cutBShapeNode = nullptr;
    cutJointB = nullptr;
#if LH_USE_BOX2D
    cutJointAB = nullptr;
#endif
}

LHRopeJointNode* LHRopeJointNode::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHRopeJointNode *ret = new LHRopeJointNode();
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
bool LHRopeJointNode::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        
        this->loadGenericInfoFromDictionary(dict);
        this->loadTransformationInfoFromDictionary(dict);

        prnt->addChild(this);

        this->loadJointInfoFromDictionary(dict);
        
        
        _thickness  = dict->floatForKey("thickness");
        _segments   = dict->intForKey("segments");
        
        _canBeCut       = dict->boolForKey("canBeCut");
        _fadeOutDelay   = dict->floatForKey("fadeOutDelay");
        _removeAfterCut = dict->boolForKey("removeAfterCut");
        
        if(dict->boolForKey("shouldDraw"))
        {
            LHDrawNode* shape = LHDrawNode::create();
            this->addChild(shape);
            ropeShape = shape;
            
            if(dict->objectForKey("relativeImagePath") && dict->boolForKey("useTexture"))
            {
            std::string imgRelPath = dict->stringForKey("relativeImagePath");
            LHScene* scene = (LHScene*)prnt->getScene();

                std::string filename = LHUtils::getLastPathComponent(imgRelPath);
                std::string foldername = LHUtils::removeLastPathComponent(imgRelPath);
                foldername = LHUtils::getLastPathComponent(foldername);
                
                
                std::string imagePath = LHUtils::getImagePathWithFilename(filename,
                                                                          foldername,
                                                                          scene->getCurrentDeviceSuffix());
                
                
                Texture2D* tex = Director::getInstance()->getTextureCache()->addImage(imagePath);
                
                if(tex){
                    Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT };
                    tex->setTexParameters(texParams);
                    ropeShape->setTexture(tex);
                }
                
            }
        }
        
        colorInfo = dict->rectForKey("colorOverlay");
        alphaValue = dict->floatForKey("alpha")/255.0f;
        colorInfo.size.height = alphaValue;
        
        _length     = dict->floatForKey("length");
        
        Point uvRep = dict->pointForKey("uvRepetitions");
        _uRepetitions = uvRep.x;
        _vRepetitions = uvRep.y;
     
        return true;
    }
    return false;
}

bool LHRopeJointNode::canBeCut(){
    return _canBeCut;
}

void LHRopeJointNode::drawRopeShape(LHDrawNode* shape,
                                    Point anchorA,
                                    Point anchorB,
                                    float length,
                                    int no_segments)
{
    if(shape)
    {
        bool isFlipped = false;
        __Array* rPoints = this->ropePointsFromPointA(anchorA, anchorB, length, no_segments, &isFlipped);
        
        __Array* sPoints = this->shapePointsFromRopePoints(rPoints, _thickness, isFlipped);
        
        LHValue* prevA = nullptr;
        LHValue* prevB = nullptr;
        float prevV = 0.0f;
        if(isFlipped){
            prevV = 1.0f;
        }
        
        float currentLength = 0;
        
        __Array* triangles = __Array::create();
        __Array* uvPoints = __Array::create();
        
        for(int i = 0; i < sPoints->count(); i+=2)
        {
            LHValue* valA = (LHValue*)sPoints->getObjectAtIndex(i);
            LHValue* valB = (LHValue*)sPoints->getObjectAtIndex(i+1);
            
            if(prevA && prevB)
            {
                Point pa = prevA->getPoint();
                Point a  = valA->getPoint();
                
                triangles->addObject(prevA);
                triangles->addObject(valA);
                triangles->addObject(valB);

                triangles->addObject(valB);
                triangles->addObject(prevA);
                triangles->addObject(prevB);

                
                currentLength += pa.getDistance(a);
                
                float texV = (currentLength/_length)*_vRepetitions;
                if(isFlipped){
                    texV = 1.0f - (currentLength/_length)*_vRepetitions;
                }

                uvPoints->addObject(LHValue::create(Point(1.0f*_uRepetitions, prevV)));
                uvPoints->addObject(LHValue::create(Point(1.0f*_uRepetitions, texV)));
                uvPoints->addObject(LHValue::create(Point(0.0f, texV)));
                
                uvPoints->addObject(LHValue::create(Point(0.0f, texV)));
                uvPoints->addObject(LHValue::create(Point(1.0f*_uRepetitions, prevV)));
                uvPoints->addObject(LHValue::create(Point(0.0f, prevV)));
                
                prevV = texV;

            }
            prevA = valA;
            prevB = valB;
        }
        
        shape->setShapeTriangles(triangles, uvPoints, Color4F(colorInfo.origin.x,
                                                              colorInfo.origin.y,
                                                              colorInfo.size.width,
                                                              alphaValue));
    }
}


void LHRopeJointNode::cutWithLineFromPointA(const Point& touchA, const Point& touchB)
{
    if(cutJointA || cutJointB) return; //dont cut again
    
    if(!this->getJoint())return;
    
    Point a = this->getAnchorA();
    Point b = this->getAnchorB();
    
    Point ptA = this->convertToNodeSpace(touchA);
    Point ptB = this->convertToNodeSpace(touchB);
    
    
    bool flipped = false;
    __Array* rPoints = this->ropePointsFromPointA(a, b, _length, _segments, &flipped);
    
    LHValue* prevValue = nullptr;
    float cutLength = 0.0f;
    
    for(size_t i = 0; i < rPoints->count(); ++i)
    {
        LHValue* val = (LHValue*)rPoints->getObjectAtIndex(i);

        if(prevValue)
        {
            Point ropeA = prevValue->getPoint();
            Point ropeB = val->getPoint();
            
            cutLength += ropeA.getDistance(ropeB);
            
            LHValue* interVal = LHUtils::LHLinesIntersection(ropeA, ropeB, ptA, ptB);
            
            if(interVal != nullptr)
            {
                Point interPt = interVal->getPoint();
                
                //need to destroy the joint and create 2 other joints
                if(this->getJoint()){
                
                    LHScene* scene = (LHScene*)this->getScene();

                    this->removeJoint();
                    
                    if(ropeShape){
                        
                        LHDrawNode* shapeA = LHDrawNode::create();
                        this->addChild(shapeA);
                        cutAShapeNode = shapeA;
                        
                        LHDrawNode* shapeB = LHDrawNode::create();
                        this->addChild(shapeB);
                        cutBShapeNode = shapeB;
                        
                        cutAShapeNode->setTexture(ropeShape->getTexture());
                        cutBShapeNode->setTexture(ropeShape->getTexture());
                        
                        ropeShape->removeFromParent();
                        ropeShape = nullptr;
                    }
                    
                    //create a new body at cut position and a joint between bodyA and this new body
                    {
#if LH_USE_BOX2D
                        LHGameWorldNode* gwNode = scene->getGameWorldNode();
                        b2World* world = gwNode->getBox2dWorld();
                        
                        
                        if(gwNode != this->getNodeA()){
                            interPt = this->getNodeA()->getParent()->convertToWorldSpace(interPt);
                            interPt = gwNode->convertToNodeSpace(interPt);
                        }
                        
                        
                        b2Vec2 bodyPos = scene->metersFromPoint(interPt);
                        
                        b2BodyDef bodyDef;
                        bodyDef.type = b2_dynamicBody;
                        bodyDef.position = bodyPos;
                        cutBodyA = world->CreateBody(&bodyDef);
                        cutBodyA->SetFixedRotation(false);
                        cutBodyA->SetGravityScale(1);
                        cutBodyA->SetSleepingAllowed(true);
                        
                        b2FixtureDef fixture;
                        fixture.density = 1.0f;
                        fixture.friction = 0.2;
                        fixture.restitution = 0.2;
                        fixture.isSensor = true;
                        
                        float radius = scene->metersFromValue(_thickness);
                        
                        b2Shape* shape = new b2CircleShape();
                        ((b2CircleShape*)shape)->m_radius = radius*0.5;
                        
                        if(shape){
                            fixture.shape = shape;
                            cutBodyA->CreateFixture(&fixture);
                        }
                        
                        if(shape){
                            delete shape;
                            shape = NULL;
                        }
                        
                        //create joint
                        b2RopeJointDef jointDef;
                        
                        jointDef.localAnchorA = scene->metersFromPoint(this->getLocalAnchorA());
                        jointDef.localAnchorB = b2Vec2(0,0);
                        
                        LHPhysicsProtocol* nodeAProt = dynamic_cast<LHPhysicsProtocol*>(this->getNodeA());
                        if(!nodeAProt)return;
                        
                        jointDef.bodyA = nodeAProt->getBox2dBody();
                        jointDef.bodyB = cutBodyA;
                        
                        if(!flipped){
                            cutJointALength = cutLength;
                        }
                        else{
                            cutJointALength = _length - cutLength;
                        }
                        
                        jointDef.maxLength         = scene->metersFromValue(cutJointALength);
                        jointDef.collideConnected  = this->getCollideConnected();
                        
                        cutJointA = (b2RopeJoint*)world->CreateJoint(&jointDef);
                        //cutJointA->SetUserData(this);
                        
#else//chipmunk
                        Node* cutBodyA = Node::create();
                        
                        PhysicsBody* body = PhysicsBody::createCircle(3);
                        cutBodyA->setPhysicsBody(body);
                        body->setDynamic(true);
                        
                        cutBodyA->setPosition(interPt);
                        
                        a = this->getLocalAnchorA();

                        this->addChild(cutBodyA);
                        
                        if(!flipped){
                            cutJointALength = cutLength;
                        }
                        else{
                            cutJointALength = _length - cutLength;
                        }
                        
                        
                        cutJointA = PhysicsJointLimit::construct(this->getNodeA()->getPhysicsBody(),
                                                                 body,
                                                                 a,
                                                                 Point(),
                                                                 0,
                                                                 cutJointALength);
                        
                        scene->getPhysicsWorld()->addJoint(cutJointA);
#endif
                    }
                    
                    //create a new body at cut position and a joint between bodyB and this new body
                    {

#if LH_USE_BOX2D
                        
                        LHScene* scene = (LHScene*)this->getScene();
                        LHGameWorldNode* gwNode = scene->getGameWorldNode();
                        b2World* world = gwNode->getBox2dWorld();
                        
                        
                        b2Vec2 bodyPos = scene->metersFromPoint(interPt);
                        b2BodyDef bodyDef;
                        bodyDef.type = b2_dynamicBody;
                        bodyDef.position = bodyPos;
                        cutBodyB = world->CreateBody(&bodyDef);
                        cutBodyB->SetFixedRotation(false);
                        cutBodyB->SetGravityScale(1);
                        cutBodyB->SetSleepingAllowed(true);
                        
                        b2FixtureDef fixture;
                        fixture.density = 1.0f;
                        fixture.friction = 0.2;
                        fixture.restitution = 0.2;
                        fixture.isSensor = true;
                        
                        float radius = scene->metersFromValue(_thickness);
                        
                        b2Shape* shape = new b2CircleShape();
                        ((b2CircleShape*)shape)->m_radius = radius*0.5;
                        
                        if(shape){
                            fixture.shape = shape;
                            cutBodyB->CreateFixture(&fixture);
                        }
                        
                        if(shape){
                            delete shape;
                            shape = NULL;
                        }
                        
                        //create joint
                        b2RopeJointDef jointDef;
                        
                        jointDef.localAnchorA = b2Vec2(0,0);
                        jointDef.localAnchorB = scene->metersFromPoint(this->getLocalAnchorB());
                        
                        
                        
                        jointDef.bodyA = cutBodyB;
                        jointDef.bodyB = LH_GET_BOX2D_BODY(this->getNodeB());
                        

                            
                        if(!flipped){
                            cutJointBLength = _length - cutLength;
                        }
                        else{
                            cutJointBLength = cutLength;
                        }
                        
                        jointDef.maxLength = scene->metersFromValue(cutJointBLength);
                        
                        jointDef.collideConnected = this->getCollideConnected();
                        
                        cutJointB = (b2RopeJoint*)world->CreateJoint(&jointDef);
                        //cutJointB->SetUserData(this);
                        
                        
                        b2RopeJointDef jointBetweenBodiesDef;
                        jointBetweenBodiesDef.localAnchorA = b2Vec2(0,0);
                        jointBetweenBodiesDef.localAnchorB = b2Vec2(0,0);
                        jointBetweenBodiesDef.bodyA = LH_GET_BOX2D_BODY(this->getNodeA());
                        jointBetweenBodiesDef.bodyB = LH_GET_BOX2D_BODY(this->getNodeB());
                        jointBetweenBodiesDef.maxLength = scene->metersFromValue(std::numeric_limits<float>::max());
                        jointBetweenBodiesDef.collideConnected = this->getCollideConnected();
                        cutJointAB = (b2RopeJoint*)world->CreateJoint(&jointBetweenBodiesDef);
                        
#else//chipmunk
                        Node* cutBodyB = Node::create();

                        PhysicsBody* body = PhysicsBody::createCircle(3);
                        
                        cutBodyB->setPhysicsBody(body);
                        body->setDynamic(true);
                        cutBodyB->setPosition(interPt);
                        
                        this->addChild(cutBodyB);

                        if(!flipped){
                            cutJointBLength = _length - cutLength;
                        }
                        else{
                            cutJointBLength = cutLength;
                        }
                        
                        b =  this->getLocalAnchorB();


                        cutJointB = PhysicsJointLimit::construct(body,
                                                                 this->getNodeB()->getPhysicsBody(),
                                                                 Point(),
                                                                 b,
                                                                 0,
                                                                 cutJointBLength);
                        
                        scene->getPhysicsWorld()->addJoint(cutJointB);
                        
#endif
                        
                    }
                }
                
                ((LHScene*)this->getScene())->didCutRopeJoint(this);
                
                return;
            }
        }
        prevValue = val;
    }
}

int LHRopeJointNode::gravityDirectionAngle(){
    Vect gravityVector = this->getScene()->getPhysicsWorld()->getGravity();
    double angle1 = atan2(gravityVector.x, -gravityVector.y);
    double angle1InDegrees = (angle1 / M_PI) * 180.0;
    int finalAngle = (360 - (int)angle1InDegrees) %  360;
    return finalAngle;
}

__Array* LHRopeJointNode::ropePointsFromPointA(Point a, Point b, float ropeLength, float numOfSegments,bool* flipped)
{
    double data[5]; /* x1 y1 x2 y2 L */
    double constants[3];  /* x0 y0 A */
    double x0, y0, A;
    double delX, delY, guess1, guess2;
    double Q, B, K;
    double step;
    
    float gravityAngle = -gravityDirectionAngle();
    Point c((a.x + b.x)*0.5, (a.y + b.y)*0.5);
    
    AffineTransform transform = AffineTransformIdentity;
    
    transform = AffineTransformTranslate(transform, c.x, c.y);
    transform = AffineTransformRotate(transform, gravityAngle);
    transform = AffineTransformTranslate(transform, -c.x, -c.y);
    

    Point ar = PointApplyAffineTransform(a, transform);
    Point br = PointApplyAffineTransform(b, transform);
    
    data[0] = ar.x;
    data[1] = ar.y; /* 1st point */
    data[2] = br.x;
    data[3] = br.y; /* 2nd point */
    
    bool ropeIsFlipped = false;
    
    if(ar.x > br.x){
        data[2] = ar.x;
        data[3] = ar.y; /* 1st point */
        data[0] = br.x;
        data[1] = br.y; /* 2nd point */
        
        Point temp = a;
        a = b;
        b = temp;
        
        ropeIsFlipped = true;
    }
    
    if(flipped)
        *flipped = ropeIsFlipped;
    
    __Array* rPoints = __Array::create();
    
    
    data[4] = ropeLength;   /* string length */
    
    delX = data[2]-data[0];
    delY = data[3]-data[1];
    /* length of string should be larger than distance
     * between given points */
    if(data[4] <= sqrt(delX * delX + delY * delY)){
        data[4] = sqrt(delX * delX + delY * delY) +0.01;
    }
    
    Q = sqrt(data[4]*data[4] - delY*delY)/delX;
    
    guess1 = log(Q + sqrt(Q*Q-1.0));
    guess2 = sqrt(6.0*(Q-1.0));
    
    B = bisection(guess1, guess2, 1e-6, f, data);
    A = delX/(2*B);
    
    K = (0.5*delY/A)/sinh(0.5*delX/A);
    x0 = data[0] + delX/2.0 - A*asinh(K);
    y0 = data[1] - A*(cosh((data[0]-x0)/A) - 1.0);
    
    //x0, y0 is the lower point of the rope
    constants[0] = x0;
    constants[1] = y0;
    constants[2] = A;
    
    
    /* write curve points on output stream stdout */
    step = (data[2]-data[0])/numOfSegments;
    
    
    transform = AffineTransformIdentity;
    transform = AffineTransformTranslate(transform, c.x, c.y);
    transform = AffineTransformRotate(transform, -gravityAngle);
    transform = AffineTransformTranslate(transform, -c.x, -c.y);
    
    Point prevPt;
    for(float x= data[0]; x <  data[2]; )
    {
        Point point(x, fcat(x, constants));
        point = PointApplyAffineTransform(point, transform);
        
        rPoints->addObject(LHValue::create(point));
        
        if(point == prevPt){
            break;//safety check
        }
        prevPt = point;
        x += step;
    }
    
    Point lastPt = prevPt;// rPoints[rPoints.size()-1];

    if(Point((int)b.x, (int)b.y) != Point((int)lastPt.x, (int)lastPt.y))
    {
        rPoints->addObject(LHValue::create(b));
    }
    
    if(!ropeIsFlipped && rPoints->count() > 0){
        
        LHValue* firstPtVal = (LHValue*)rPoints->getObjectAtIndex(0);
        Point firstPt = firstPtVal->getPoint();
        
        if(Point((int)a.x, (int)a.y) != Point((int)firstPt.x, (int)firstPt.y))
        {
            rPoints->insertObject(LHValue::create(a), 0);
        }

        if(a != firstPt)
        {
            rPoints->insertObject(LHValue::create(a), 0);
        }
    }
    
    return rPoints;
}

__Array* LHRopeJointNode::shapePointsFromRopePoints(__Array* rPoints, float thick, bool flipped)
{
    __Array* shapePoints = __Array::create();

    bool added = false;
    
    LHValue* prvVal = nullptr;
    
    ssize_t count = rPoints->count();
    
    LHValue* lastPt = nullptr;
    if(count > 0){
        lastPt = (LHValue*)rPoints->getObjectAtIndex(count-1);
    }
    
    for(size_t i = 0; i < count; ++i)
    {
        LHValue* ptVal = (LHValue*)rPoints->getObjectAtIndex(i);
        
        if(prvVal != nullptr)
        {
            Point prevPt = prvVal->getPoint();
            Point pt = ptVal->getPoint();
            
            __Array* points = thickLinePointsFrom(prevPt, pt, thick);
            
            
            if((ptVal == lastPt) && !added){
                if(flipped){
                    LHValue* ptVal0 = (LHValue*)points->getObjectAtIndex(0);
                    shapePoints->addObject(ptVal0);
                    
                    LHValue* ptVal1 = (LHValue*)points->getObjectAtIndex(1);
                    shapePoints->addObject(ptVal1);
                }
                else{
                    LHValue* ptVal1 = (LHValue*)points->getObjectAtIndex(1);
                    shapePoints->addObject(ptVal1);
                    
                    LHValue* ptVal0 = (LHValue*)points->getObjectAtIndex(0);
                    shapePoints->addObject(ptVal0);
                }
                added = true;
            }
            else{
                if(flipped){
                    LHValue* ptVal2 = (LHValue*)points->getObjectAtIndex(2);
                    shapePoints->addObject(ptVal2);
                    
                    LHValue* ptVal3 = (LHValue*)points->getObjectAtIndex(3);
                    shapePoints->addObject(ptVal3);
                }
                else{
                    LHValue* ptVal3 = (LHValue*)points->getObjectAtIndex(3);
                    shapePoints->addObject(ptVal3);
                    
                    LHValue* ptVal2 = (LHValue*)points->getObjectAtIndex(2);
                    shapePoints->addObject(ptVal2);
                }
            }
        }
        prvVal = ptVal;
    }
    
    return shapePoints;
}

__Array* LHRopeJointNode::thickLinePointsFrom(const Point& start, const Point& end, const float& width)
{
    float dx = start.x - end.x;
    float dy = start.y - end.y;
    
    Point rightSide = Point(dy, -dx);

    if (rightSide.getLength() > 0) {
        rightSide = LHUtils::LHPointNormalize(rightSide);
        rightSide = LHUtils::LHPointScaled(rightSide, width*0.5);
    }
    
    Point leftSide(-dy, dx);
    if (leftSide.getLength() > 0) {
        leftSide = LHUtils::LHPointNormalize(leftSide);
        leftSide = LHUtils::LHPointScaled(leftSide, width*0.5);
    }
    
    Point one    = leftSide + start;
    Point two    = rightSide+ start;
    Point three  = rightSide+ end;
    Point four   = leftSide + end;
    
    __Array* array = __Array::create();

    //G+B
    array->addObject(LHValue::create(four));
    array->addObject(LHValue::create(three));
    
    //C+P
    array->addObject(LHValue::create(one));
    array->addObject(LHValue::create(two));

    return array;
}


#if COCOS2D_VERSION >= 0x00030200
void LHRopeJointNode::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
#else
void LHRopeJointNode::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
#endif
{
    
    if(!this->getNodeA() || !this->getNodeB())return;
    
    
    Point anchorA = this->getAnchorA();
    Point anchorB = this->getAnchorB();
    
    if(ropeShape){
        drawRopeShape(ropeShape, anchorA, anchorB, _length, _segments);
    }
    
    if(_removeAfterCut && cutAShapeNode && cutBShapeNode){

        if(_cutTimer == -1)
        {
            _cutTimer = LHUtils::LHMillisecondNow();
        }
        unsigned long long currentTimer = LHUtils::LHMillisecondNow();
        
        float unit = (currentTimer - _cutTimer)/(_fadeOutDelay*1000.0);
        
        alphaValue = colorInfo.size.height;
        alphaValue -= alphaValue*unit;
        
        if(unit >=1.0f)
        {
            this->removeFromParent();
            return;
        }
    }

#if LH_USE_BOX2D

    LHScene* scene = (LHScene*)this->getScene();
    LHGameWorldNode* gWNode = scene->getGameWorldNode();
    
    if(cutAShapeNode){

        b2Vec2 pos = cutBodyA->GetPosition();
        Point gwPos = scene->pointFromMeters(pos);
        Point worldPoint = gWNode->convertToWorldSpace(gwPos);
        
        Point B = this->convertToNodeSpaceAR(worldPoint);

        this->drawRopeShape(cutAShapeNode, anchorA, B, cutJointALength, _segments);
    }
    
    if(cutBShapeNode){
        b2Vec2 pos = cutBodyB->GetPosition();
        LHScene* scene = (LHScene*)this->getScene();
        
        Point gwPos = scene->pointFromMeters(pos);
        
        Point worldPoint = gWNode->convertToWorldSpace(gwPos);
        Point A = this->convertToNodeSpace(worldPoint);
        
        
        
        this->drawRopeShape(cutBShapeNode, A, anchorB, cutJointBLength, _segments);
    }
    
#else
    if(cutAShapeNode){
        
        Point pt = cutJointA->getBodyB()->getNode()->convertToWorldSpaceAR(Point());
        Point B = this->getNodeA()->getParent()->convertToNodeSpaceAR(pt);
        
        this->drawRopeShape(cutAShapeNode, anchorA, B, cutJointALength, _segments);
    }
    
    if(cutBShapeNode){
        
        Point pt = cutJointB->getBodyA()->getNode()->convertToWorldSpaceAR(Point());
        Point A = this->getNodeB()->getParent()->convertToNodeSpaceAR(pt);
        
        this->drawRopeShape(cutBShapeNode, A, anchorB, cutJointBLength, _segments);
    }
#endif
    
    if(renderer)
    {
#if COCOS2D_VERSION >= 0x00030200
        Node::visit(renderer, parentTransform, parentFlags);
#else
        Node::visit(renderer, parentTransform, parentTransformUpdated);
#endif
    }
}

bool LHRopeJointNode::lateLoading()
{
    this->findConnectedNodes();
    
    Node* nodeA = this->getNodeA();
    Node* nodeB = this->getNodeB();
    
    Point relativePosA = this->getLocalAnchorA();
    Point relativePosB = this->getLocalAnchorB();
    
    if(nodeA && nodeB)
    {
        //this ensures that all transformations are set on the body prior creating the joint
        nodeA->setPosition(nodeA->getPosition());
        nodeB->setPosition(nodeB->getPosition());


#if LH_USE_BOX2D
        LHScene* scene = (LHScene*)this->getScene();
        LHGameWorldNode* pNode = scene->getGameWorldNode();
        b2World* world = pNode->getBox2dWorld();
        if(world == nullptr)return false;
        
        b2Body* bodyA = LH_GET_BOX2D_BODY(this->getNodeA());
        b2Body* bodyB = LH_GET_BOX2D_BODY(this->getNodeB());
        
        if(!bodyA || !bodyB)return false;
        
        b2Vec2 posA = scene->metersFromPoint(relativePosA);
        b2Vec2 posB = scene->metersFromPoint(relativePosB);
        
        b2RopeJointDef jointDef;
        
        jointDef.localAnchorA = posA;
        jointDef.localAnchorB = posB;
        
        jointDef.bodyA = bodyA;
        jointDef.bodyB = bodyB;
        
        jointDef.maxLength = scene->metersFromValue(_length);
        
        jointDef.collideConnected = this->getCollideConnected();
        
        b2RopeJoint* joint = (b2RopeJoint*)world->CreateJoint(&jointDef);
        joint->SetUserData(this);
        this->setJoint(joint);
        
#else//chipmunk
        if(nodeA->getPhysicsBody() && nodeB->getPhysicsBody())
        {
            PhysicsJointLimit* joint = PhysicsJointLimit::construct(nodeA->getPhysicsBody(),
                                                                    nodeB->getPhysicsBody(),
                                                                    relativePosA,
                                                                    relativePosB,
                                                                    0,
                                                                    _length);

            this->getScene()->getPhysicsWorld()->addJoint(joint);

            this->setJoint(joint);
            
            return true;
        }
#endif
        
        
    }

    return false;
}
