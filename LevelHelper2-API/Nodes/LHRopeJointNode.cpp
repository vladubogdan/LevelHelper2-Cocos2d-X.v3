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
#include "LHPointValue.h"


double bisection(double g0, double g1, double epsilon,
                 double (*fp)(double, void *), void *data)
{
    if(!data)return 0;
    
    double v0, v1, g, v;
    v0 = fp(g0, data);
    v1 = fp(g1, data);
    
    while(fabs(g1-g0) > fabs(epsilon)){
        g = (g0+g1)/2.0;
        v = fp(g, data);
        if(v == 0.0)
            return g;
        else if(v*v0 < 0.0){
            g1 = g;   v1 = v;
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
    joint = nullptr;
    nodeA = nullptr;
    nodeB = nullptr;
    ropeShape = nullptr;
    cutAShapeNode = nullptr;
    cutBShapeNode = nullptr;
    _cutTimer = -1;
}

LHRopeJointNode::~LHRopeJointNode()
{
    nodeA = nullptr;
    nodeB = nullptr;

    ropeShape = nullptr;
    joint = nullptr;
    
    cutAShapeNode = nullptr;
    cutJointA = nullptr;
    
    cutBShapeNode = nullptr;
    cutJointB = nullptr;

}

LHRopeJointNode* LHRopeJointNode::ropeJointNodeWithDictionary(LHDictionary* dict, Node* prnt)
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
        
        loadGenericInfoFromDictionary(dict);
        
        prnt->addChild(this);
        
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
            ropeShape->setLocalZOrder(1);
            
            if(dict->objectForKey("relativeImagePath"))
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
        colorInfo.size.height = dict->floatForKey("alpha")/255.0f;
        alphaValue = dict->floatForKey("alpha")/255.0f;
        
        
        this->setLocalZOrder(dict->intForKey("zOrder"));

        _nodeAUUID = dict->stringForKey("spriteAUUID");
        _nodeBUUID = dict->stringForKey("spriteBUUID");
        
        _relativePosA = dict->pointForKey("relativePosA");
        _relativePosB = dict->pointForKey("relativePosB");
        
        _length     = dict->floatForKey("length");
        
        Point uvRep = dict->pointForKey("uvRepetitions");
        _uRepetitions = uvRep.x;
        _vRepetitions = uvRep.y;
     
        return true;
    }
    return false;
}


Point LHRopeJointNode::getAnchorA()
{
    Point pt = nodeA->convertToWorldSpaceAR(Point(_relativePosA.x, -_relativePosA.y));
    return nodeA->getParent()->convertToNodeSpaceAR(pt);
}

Point LHRopeJointNode::getAnchorB()
{
    Point pt = nodeB->convertToWorldSpaceAR(Point(_relativePosB.x, -_relativePosB.y));
    return nodeB->getParent()->convertToNodeSpaceAR(pt);
}


bool LHRopeJointNode::canBeCut(){
    return _canBeCut;
}

void LHRopeJointNode::drawRopeShape(LHDrawNode* shape, Point anchorA, Point anchorB, float length, int no_segments)
{
    if(shape)
    {
        bool isFlipped = false;
        __Array* rPoints = this->ropePointsFromPointA(anchorA, anchorB, length, no_segments, &isFlipped);
        
        __Array* sPoints = this->shapePointsFromRopePoints(rPoints, _thickness, isFlipped);
        
        LHPointValue* prevA = nullptr;
        LHPointValue* prevB = nullptr;
        float prevV = 0.0f;
        if(isFlipped){
            prevV = 1.0f;
        }
        
        float currentLength = 0;
        
        __Array* triangles = __Array::create();
        __Array* uvPoints = __Array::create();
        
        for(int i = 0; i < sPoints->count(); i+=2)
        {
            LHPointValue* valA = (LHPointValue*)sPoints->getObjectAtIndex(i);
            LHPointValue* valB = (LHPointValue*)sPoints->getObjectAtIndex(i+1);
            
            if(prevA && prevB)
            {
                Point pa = prevA->getValue();
                Point a  = valA->getValue();
                
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

                uvPoints->addObject(LHPointValue::create(Point(1.0f*_uRepetitions, prevV)));
                uvPoints->addObject(LHPointValue::create(Point(1.0f*_uRepetitions, texV)));
                uvPoints->addObject(LHPointValue::create(Point(0.0f, texV)));
                
                uvPoints->addObject(LHPointValue::create(Point(0.0f, texV)));
                uvPoints->addObject(LHPointValue::create(Point(1.0f*_uRepetitions, prevV)));
                uvPoints->addObject(LHPointValue::create(Point(0.0f, prevV)));
                
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


void LHRopeJointNode::cutWithLineFromPointA(const Point& ptA, const Point& ptB)
{
    if(cutJointA || cutJointB) return; //dont cut again
    
    if(!joint)return;
    
    Point a = this->getAnchorA();
    Point b = this->getAnchorB();
    
    LHScene* scene = (LHScene*)this->getScene();
    
    bool flipped = false;
    __Array* rPoints = this->ropePointsFromPointA(a, b, _length, _segments, &flipped);
    
    LHPointValue* prevValue = nullptr;
    float cutLength = 0.0f;

    for(size_t i = 0; i < rPoints->count(); ++i)
    {
        LHPointValue* val = (LHPointValue*)rPoints->getObjectAtIndex(i);

        if(prevValue)
        {
            Point ropeA = prevValue->getValue();
            Point ropeB = val->getValue();
            
            cutLength += ropeA.getDistance(ropeB);
            
            LHPointValue* interVal = LHUtils::LHLinesIntersection(ropeA, ropeB, ptA, ptB);
            
            if(interVal != nullptr)
            {
                Point interPt = interVal->getValue();
                
                //need to destroy the joint and create 2 other joints
                if(joint){
                    
                    _cutTimer = LHUtils::LHMillisecondNow();
                    
                    nodeA = joint->getBodyA()->getNode();
                    nodeB = joint->getBodyB()->getNode();

                    float length = _length;

                    joint->removeFormWorld();
                    joint = nullptr;
                    
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
                        Node* cutBodyA = Node::create();
                        
                        PhysicsBody* body = PhysicsBody::createCircle(3);
                        cutBodyA->setPhysicsBody(body);
                        body->setDynamic(true);
                        
                        cutBodyA->setPosition(interPt);
                        //body->setDensity(0.1);
//                        cutBodyA.physicsBody.sensor = YES;
                        
                        a = Point(_relativePosA.x, -_relativePosA.y);

                        this->addChild(cutBodyA);
                        
                        if(!flipped){
                            cutJointALength = cutLength;
                        }
                        else{
                            cutJointALength = length - cutLength;
                        }
                        
                        cutJointA = PhysicsJointLimit::construct(nodeA->getPhysicsBody(),
                                                                 body,
                                                                 a,
                                                                 Point(),
                                                                 0,
                                                                 cutJointALength);
                        
                        scene->getPhysicsWorld()->addJoint(cutJointA);
                    }
                    
                    //create a new body at cut position and a joint between bodyB and this new body
                    {

                        Node* cutBodyB = Node::create();

                        PhysicsBody* body = PhysicsBody::createCircle(3);
                        
                        cutBodyB->setPhysicsBody(body);
                        body->setDynamic(true);
                        cutBodyB->setPosition(interPt);
                        
//                        cutBodyB.physicsBody.density = 0.1;
//                        cutBodyB.physicsBody.sensor = YES;
                        
                        this->addChild(cutBodyB);

                        if(!flipped){
                            cutJointBLength = length - cutLength;
                        }
                        else{
                            cutJointBLength = cutLength;
                        }
                        
                        b = Point(_relativePosB.x, -_relativePosB.y);


                        cutJointB = PhysicsJointLimit::construct(body,
                                                                 nodeB->getPhysicsBody(),
                                                                 Point(),
                                                                 b,
                                                                 0,
                                                                 cutJointBLength);
                        
                        scene->getPhysicsWorld()->addJoint(cutJointB);
                    }
                }
                
                return;
            }
        }
        prevValue = val;
    }
}

void LHRopeJointNode::removeFromParent(){

    if(cutJointA){
        cutJointA->removeFormWorld();
        cutJointA = nullptr;
    }
    
    if(cutJointB){
        cutJointB->removeFormWorld();
        cutJointB = nullptr;
    }
    
    if(joint){
        joint->removeFormWorld();
        joint = nullptr;
    }
    
    Node::removeFromParent();
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
    double step, x;
    
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
    x = data[0];
    for(float x= data[0]; x <  data[2]; )
    {
        Point point(x, fcat(x, constants));
        point = PointApplyAffineTransform(point, transform);
        
        rPoints->addObject(LHPointValue::create(point));
        
        if(point == prevPt){
            break;//safety check
        }
        prevPt = point;
        x += step;
    }
    
    Point lastPt = prevPt;// rPoints[rPoints.size()-1];

    if(Point((int)b.x, (int)b.y) != Point((int)lastPt.x, (int)lastPt.y))
    {
        rPoints->addObject(LHPointValue::create(b));
    }
    
    if(!ropeIsFlipped && rPoints->count() > 0){
        
        LHPointValue* firstPtVal = (LHPointValue*)rPoints->getObjectAtIndex(0);
        Point firstPt = firstPtVal->getValue();
        
        if(Point((int)a.x, (int)a.y) != Point((int)firstPt.x, (int)firstPt.y))
        {
            rPoints->insertObject(LHPointValue::create(a), 0);
        }

        if(a != firstPt)
        {
            rPoints->insertObject(LHPointValue::create(a), 0);
        }
    }
    
    return rPoints;
}

__Array* LHRopeJointNode::shapePointsFromRopePoints(__Array* rPoints, float thick, bool flipped)
{
    __Array* shapePoints = __Array::create();

    bool first = true;
    bool added = false;
    
    LHPointValue* prvVal = nullptr;
    
    int count = rPoints->count();
    
    LHPointValue* lastPt = nullptr;
    if(count > 0){
        lastPt = (LHPointValue*)rPoints->getObjectAtIndex(count-1);
    }
    
    for(size_t i = 0; i < count; ++i)
    {
        LHPointValue* ptVal = (LHPointValue*)rPoints->getObjectAtIndex(i);
        
        if(prvVal != nullptr)
        {
            Point prevPt = prvVal->getValue();
            Point pt = ptVal->getValue();
            
            __Array* points = thickLinePointsFrom(prevPt, pt, thick);
            
            
            if((ptVal == lastPt) && !added){
                if(flipped){
                    LHPointValue* ptVal0 = (LHPointValue*)points->getObjectAtIndex(0);
                    shapePoints->addObject(ptVal0);
                    
                    LHPointValue* ptVal1 = (LHPointValue*)points->getObjectAtIndex(1);
                    shapePoints->addObject(ptVal1);
                }
                else{
                    LHPointValue* ptVal1 = (LHPointValue*)points->getObjectAtIndex(1);
                    shapePoints->addObject(ptVal1);
                    
                    LHPointValue* ptVal0 = (LHPointValue*)points->getObjectAtIndex(0);
                    shapePoints->addObject(ptVal0);
                }
                added = true;
            }
            else{
                if(flipped){
                    LHPointValue* ptVal2 = (LHPointValue*)points->getObjectAtIndex(2);
                    shapePoints->addObject(ptVal2);
                    
                    LHPointValue* ptVal3 = (LHPointValue*)points->getObjectAtIndex(3);
                    shapePoints->addObject(ptVal3);
                }
                else{
                    LHPointValue* ptVal3 = (LHPointValue*)points->getObjectAtIndex(3);
                    shapePoints->addObject(ptVal3);
                    
                    LHPointValue* ptVal2 = (LHPointValue*)points->getObjectAtIndex(2);
                    shapePoints->addObject(ptVal2);
                }
            }
            first = false;
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
    array->addObject(LHPointValue::create(four));
    array->addObject(LHPointValue::create(three));
    
    //C+P
    array->addObject(LHPointValue::create(one));
    array->addObject(LHPointValue::create(two));

    return array;
}

void LHRopeJointNode::visit(Renderer *renderer,
                            const Mat4& parentTransform,
                            bool parentTransformUpdated)
{
    Point anchorA = getAnchorA();
    Point anchorB = getAnchorB();
    
    
    if(isnan(anchorA.x) || isnan(anchorA.y) || isnan(anchorB.x) || isnan(anchorB.y)){
        return;
    }

    if(ropeShape){
        drawRopeShape(ropeShape, anchorA, anchorB, _length, _segments);
    }
    
    long currentTimer = LHUtils::LHMillisecondNow();
    
    if(_removeAfterCut && cutAShapeNode && cutBShapeNode){
        
        float unit = (currentTimer - _cutTimer)/_fadeOutDelay;
        alphaValue = colorInfo.size.height;
        alphaValue -= alphaValue*unit;
        
        if(unit >=1){
            this->removeFromParent();
            return;
        }
    }

    if(cutAShapeNode){
        Point pt = cutJointA->getBodyB()->getNode()->convertToWorldSpaceAR(Point());
        Point B = nodeA->getParent()->convertToNodeSpaceAR(pt);
        
        this->drawRopeShape(cutAShapeNode, anchorA, B, cutJointALength, _segments);
    }
    
    if(cutBShapeNode){
        
        Point pt = cutJointB->getBodyA()->getNode()->convertToWorldSpaceAR(Point());
        Point A = nodeB->getParent()->convertToNodeSpaceAR(pt);
        
        this->drawRopeShape(cutBShapeNode, A, anchorB, cutJointBLength, _segments);
    }

    Node::visit(renderer, parentTransform, parentTransformUpdated);
}

bool LHRopeJointNode::lateLoading()
{
    if(_nodeAUUID.length() == 0 || _nodeBUUID.length() == 0)
        return false;
    
    LHScene* scene = (LHScene*)this->getScene();
    
    if(LHScene::isLHScene(scene))
    {
        nodeA = scene->getChildNodeWithUUID(_nodeAUUID);
        nodeB = scene->getChildNodeWithUUID(_nodeBUUID);
    }
    
    if(nodeA && nodeB && nodeA->getPhysicsBody() && nodeB->getPhysicsBody())
    {
        joint = PhysicsJointLimit::construct(nodeA->getPhysicsBody(),
                                             nodeB->getPhysicsBody(),
                                             Point(_relativePosA.x,
                                                   -_relativePosA.y),
                                             Point(_relativePosB.x,
                                                   -_relativePosB.y),
                                             0,
                                             _length);

        scene->getPhysicsWorld()->addJoint(joint);

        return true;
    }
    return false;
}
