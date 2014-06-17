//
//  LHWater.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHWater.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHDevice.h"
#include "LHPointValue.h"


#if LH_USE_BOX2D
#include "LHb2BuoyancyController.h"
#endif



class LHWave
{
public:
    
    LHWave(){
        waveBlock = nullptr;
        wavelength= 0;
        amplitude = 0;
        startAmplitude = 0;
        currentStep = 0;
        totalSteps = 0;
        offset = 0;
        increment = 0;
        decayBlock = nullptr;
        clampBlock = nullptr;
        velocity = 0;
        left = 0;
        right = 1000;
        halfWidth = 0;
        middle = 0;
    }
    
    virtual ~LHWave(){}
    
    float wavelength, amplitude, startAmplitude, offset, increment, velocity;
    float left, right, halfWidth, middle;
    int currentStep, totalSteps;
    
    typedef float (*decayClampFuncPtrType)(float, float, float, float);
    typedef float (*waveFuncPtrType) (float);
    
    decayClampFuncPtrType decayBlock;
    decayClampFuncPtrType clampBlock;
    waveFuncPtrType waveBlock;
    
    void setDecayBlock( decayClampFuncPtrType dBlock ){
        decayBlock = dBlock;
    }
    decayClampFuncPtrType getDecayBlock(){return decayBlock;}
    
    
    void setClampBlock( decayClampFuncPtrType cBlock ){
        clampBlock = cBlock;
    }
    decayClampFuncPtrType getClampBlock(){return clampBlock;}
    
    
    void setWaveBlock( waveFuncPtrType wBlock ){
        waveBlock = wBlock;
    }
    waveFuncPtrType getWaveBlock(){return waveBlock;}
    

    static float linearEase(float t, float b, float c, float d){
        return b + c * t  / d;
    }
    static float sinEaseIn(float t, float b, float c, float d){
        return -c * cos(t/d * (M_PI / 2)) + c + b;
    }
    static float sinEaseOut(float t, float b, float c, float d){
        return c * sin(t / d * (M_PI / 2)) + b;
    }
    static float sinEaseInOut(float t, float b, float c, float d){
        return -c / 2 * (cos(M_PI * t / d) - 1) + b;
    }

    
    float valueAt(float position)
    {
        if(position < left || position > right) {
            return 0.0f;
        }
        float v = (waveBlock != nullptr ? waveBlock((position - left + offset) / wavelength) : 1.0f) * amplitude;
        if(clampBlock != nullptr) {
            v = clampBlock(abs(position - middle), v, -v, halfWidth);
        }
        return v;
    }
    void step()
    {
        if(decayBlock != nullptr) {
            amplitude = decayBlock(currentStep, startAmplitude, -startAmplitude, totalSteps);
            currentStep++;
        }
        offset  += increment;
        left    += velocity;
        right   += velocity;
        halfWidth = (right - left) / 2;
        middle  = left + halfWidth;
    }

};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

LHWater::LHWater()
{

}

LHWater::~LHWater()
{
    _drawNode = nullptr;
    clearWaves();
}

LHWater* LHWater::waterWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHWater *ret = new LHWater();
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

bool LHWater::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        
        loadGenericInfoFromDictionary(dict);
        
        prnt->addChild(this);
       
        
        _drawNode = DrawNode::create();
        this->addChild(_drawNode);
        
        
        _width      = dict->floatForKey("width");
        _height     = dict->floatForKey("height");
        _numLines   = dict->floatForKey("numLines");
        
        //_waves = [[NSMutableArray alloc] init];
        _waterDensity = dict->floatForKey("waterDensity");
        _lineWidth = _width / _numLines;
        
        _turbulence = dict->boolForKey("turbulence");
        _turbulenceL= dict->floatForKey("turbulenceL");
        _turbulenceH= dict->floatForKey("turbulenceH");
        _turbulenceV= dict->floatForKey("turbulenceV");
        _turbulenceVelocity = dict->floatForKey("turbulenceVelocity");
        
        _splashCollision = dict->boolForKey("splash");
        _splashHeight    = dict->floatForKey("splashH");
        _splashWidth     = dict->floatForKey("splashW");
        _splashTime      = dict->floatForKey("splashT");

    
        this->setContentSize(dict->sizeForKey("size"));
        
        Point unitPos   = dict->pointForKey("generalPosition");
        Point pos       = LHScene::positionForNode(this, unitPos);
        
        LHDictionary* devPositions = dict->dictForKey("devicePositions");
        if(devPositions)
        {
            std::string unitPosStr = LHDevice::devicePosition(devPositions, LH_SCREEN_RESOLUTION);
            
            if(unitPosStr.length()>0){
                Point unitPos = PointFromString(unitPosStr);
                pos = LHScene::positionForNode(this, unitPos);
            }
        }
        
        float alpha = dict->floatForKey("alpha");
        this->setOpacity(alpha);
        
//        this->setRotation(dict->floatForKey("rotation"));
//        this->setZOrder(dict->floatForKey("zOrder"));
        
        Point scl = dict->pointForKey("scale");
        this->setScaleX(scl.x);
        this->setScaleY(scl.y);
        
        this->setPosition(pos);

        _drawNode->clear();
        
        Color3B overlay = dict->colorForKey("colorOverlay");
        colorOverlay = Color4F(overlay.r/255.0f, overlay.g/255.0f, overlay.b/255.0f, alpha/255.0f);

        this->createTurbulence();
        
        return true;
    }
    return false;
}

void LHWater::createTurbulence(){
    clearWaves();
    if(_turbulence)
        createTurbulance(_turbulenceH, _turbulenceV, _turbulenceL);
}
void LHWater::clearWaves(){
    for(size_t i = 0; i < waves.size();++i){
        LHWave* wv = waves[i];
        delete wv;
    }
    waves.clear();
}
LHWave* LHWater::createTurbulance(float h, float v, float l)
{
    LHWave* wv = new LHWave();
    
    wv->setWaveBlock(&sinf);

    wv->left = 0;
    wv->right = _width;
    wv->wavelength = l;
    wv->increment = v;
    wv->amplitude = h;
    this->addWave(wv);
    return wv;
}
void LHWater::addWave(LHWave* w)
{
    w->startAmplitude = w->amplitude;
    waves.push_back(w);
}

float LHWater::valueAt(float pos)
{
    float y = 0;
    for(size_t i = 0; i < waves.size(); ++i) {
        LHWave* wv = waves[i];
        y += wv->valueAt(pos);
    }
    return y;
}

float LHWater::globalXToWaveX(float pointX){
    
    Size sizet = getContentSize();
    Point pos  = getPosition();
    
    float fullWidth = sizet.width*getScaleX();
    float halfWidth = fullWidth*0.5;
    
    float val = (pointX - pos.x) + halfWidth;
    float percent = val/fullWidth;
    
    return _width*percent;
}


float LHWater::waveYToGlobalY(float waveY){
    
    Size sizet = getContentSize();
    Point pos  = getPosition();
    
    float waveHeight = _height+waveY;
    float percent = waveHeight/_height;
    
    float fullHeight = sizet.height*getScaleY();;
    float halfHeight = fullHeight*0.5;
    
    float result = pos.y - fullHeight*percent + halfHeight;
    return result;
}


std::vector<LHWave*> LHWater::createSplash(float pos, float h, float w, float t)
{
    LHWave* w1 = new LHWave();
    LHWave* w2 = new LHWave();
    
    w1->setWaveBlock(&cosf);
    w2->setWaveBlock(&cosf);
    
    w1->setDecayBlock(&LHWave::sinEaseOut);
    w2->setDecayBlock(&LHWave::sinEaseOut);
    
    w1->setClampBlock(&LHWave::sinEaseInOut);
    w2->setClampBlock(&LHWave::sinEaseInOut);
    
    w1->amplitude = w2->amplitude = h / 2;
    float hw = w / 2;
    w1->left = w2->left = pos - hw;
    w1->right = w2->right = w1->left + w;
    w1->totalSteps = w2->totalSteps = t;
    w1->wavelength = w2->wavelength = hw / (3 * M_PI);
    float v = hw / t;
    w1->velocity = v;
    w2->velocity = -v;
    this->addWave(w1);
    this->addWave(w2);
    
    std::vector<LHWave*> currentWaves;
    currentWaves.push_back(w1);
    currentWaves.push_back(w2);
    return currentWaves;
}

void LHWater::setShapeTriangles(__Array* triangles, const Color4F& c4)
{
    _drawNode->clear();
    
    for(int i = 0; i < triangles->count(); i+=3)
    {
        LHPointValue* valA = (LHPointValue*)triangles->getObjectAtIndex(i);
        LHPointValue* valB = (LHPointValue*)triangles->getObjectAtIndex(i+1);
        LHPointValue* valC = (LHPointValue*)triangles->getObjectAtIndex(i+2);
        
        Point pA = valA->getValue();
        pA.y = -pA.y;
        
        Point pB = valB->getValue();
        pB.y = -pB.y;
        
        Point pC = valC->getValue();
        pC.y = -pC.y;
        
        _drawNode->drawTriangle(pA, pB, pC, c4);
    }
}

void LHWater::visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated)
{
    std::vector<LHWave*> toKeep;
    for(size_t i = 0; i < waves.size(); ++i)
    {
        LHWave* w = waves[i];
        w->step();
        
        bool done = w->getDecayBlock() != nullptr && (w->currentStep >= w->totalSteps);
        bool outBounds = (w->right <= 0 && w->velocity <= 0) || (w->left >= _width && w->velocity >= 0);
        if(done || outBounds) {
            delete w;
        }
        else{
            toKeep.push_back(w);
        }
    }
    waves.clear();
    for(size_t i = 0; i < toKeep.size();++i){
        LHWave* w = toKeep[i];
        waves.push_back(w);
    }
    
    
    std::vector<float> points;
    for(int i = 0; i <= _numLines; ++i) {
        points.push_back(this->valueAt(i*_lineWidth));
    }
    
    float ox = -_width*0.5;
    float oy = -_height*0.5;
    
    if(points.size() > 0)
    {
        __Array* trianglePts = __Array::create();
        
        float firstX = -_width*0.5;
        float firstY = _height*0.5;
        
        LHPointValue* first = LHPointValue::create(Point(firstX, firstY));
        trianglePts->addObject(first);
        
        LHPointValue* prevDown = first;
        
        float x = -_width*0.5;
        float y = points[0] + oy;
        
        trianglePts->addObject(LHPointValue::create(Point(x, y)));

        LHPointValue* prev = nullptr;
        
        for(int i = 1; i < (int)points.size(); ++i)
        {
            x = _lineWidth*i - _width*0.5;
            y = points[i] - _height*0.5;
            
            if(prev){
                trianglePts->addObject(first);
                trianglePts->addObject(prev);
            }
            
            LHPointValue* curValue = LHPointValue::create(Point(x, y));
            trianglePts->addObject(curValue);
            
            trianglePts->addObject(curValue);
            
            
            LHPointValue* down = LHPointValue::create(Point(x, firstY));
            trianglePts->addObject(down);
            trianglePts->addObject(prevDown);
            prevDown = down;
            first = down;
            
            prev = curValue;
        }
        
        x = _width*0.5;
        y = _height*0.5;
        trianglePts->addObject(LHPointValue::create(Point(x, y)));
        
        x = _width + ox;
        y = -_height*0.5;
        trianglePts->addObject(LHPointValue::create(Point(x, y)));
        trianglePts->addObject(first);
        

        this->setShapeTriangles(trianglePts, colorOverlay);
        
    }
    Node::visit(renderer, parentTransform, parentTransformUpdated);

    
//#if LH_USE_BOX2D
//    
//    LHScene* scene = [self scene];
//    LHPhysicsNode* pNode = [scene gameWorldNode];
//    b2World* world = [pNode box2dWorld];
//    
//    if(NULL == buoyancyController && world){
//        buoyancyController = new LH_b2BuoyancyController(world);
//        buoyancyController->offset = 27;
//    }
//    
//    buoyancyController->density = -waterDensity;
//    
//    if(world){
//        for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()){
//            
//            if(b->GetType() == b2_dynamicBody){
//                
//                CGRect rect = [self waveRect];
//                
//                rect.origin.y -= 100;
//                
//                CGRect bodyRect = [self  boundingRectForBody:b];
//                
//                LHNodePhysicsProtocolImp* nodePhy = LH_ID_BRIDGE_CAST(b->GetUserData());
//                CCNode* node = [nodePhy node];
//                
//                if(node){
//                    
//                    float scaleX = [node scaleX];
//                    float scaleY = [node scaleY];
//                    
//                    CGPoint bodyPt = [scene pointFromMeters:b->GetPosition()];
//                    
//                    float x = bodyPt.x;
//                    float y = [self valueAt:[self globalXToWaveX:x]];
//                    
//                    y = [self waveYToGlobalY:y];
//                    
//                    rect.origin.y = y - bodyRect.size.height*0.5;
//                    
//                    if(LHRectOverlapsRect(rect,  bodyRect))
//                    {
//                        
//                        BOOL addedSplash = false;
//                        float previousX = -1000000;
//                        b2Fixture* f = b->GetFixtureList();
//                        while (f) {
//                            
//                            if(!f->IsSensor())
//                            {
//                                {
//                                    y = [scene metersFromValue:y];
//                                    
//                                    buoyancyController->offset = y;
//                                    buoyancyController->useDensity = NO;
//                                    buoyancyController->velocity = b2Vec2(0,0);
//                                    if(turbulence){
//                                        float vdirection = 1.0f;
//                                        if(turbulenceV > 0)
//                                            vdirection = -1.0f;
//                                            buoyancyController->velocity = b2Vec2(vdirection*turbulenceVelocity, 0);
//                                            }
//                                    
//                                    NSString* hadSplash = [bodySplashes objectForKey:[NSString stringWithFormat:@"%p", b->GetUserData()]];
//                                    
//                                    bool after = buoyancyController->ApplyToFixture(f);
//                                    if(after && hadSplash == nil && splashCollision){
//                                        
//                                        float splashX = [self globalXToWaveX:x];
//                                        if(previousX != splashX && !addedSplash){
//                                            NSArray* splashes = [self createSplash:splashX h:splashHeight*scaleY w:splashWidth*scaleX t:splashTime];
//                                            previousX = splashX;
//                                            addedSplash = true;
//                                            int i = 0;
//                                            for(LHWave* wave in splashes){
//                                                [wave step];
//                                                ++i;
//                                                // if(i > 2)break;
//                                            }
//                                            
//                                            [bodySplashes setObject:[NSNumber numberWithBool:YES] forKey:[NSString stringWithFormat:@"%p", b->GetUserData()]];
//                                        }
//                                    }
//                                    buoyancyController->ApplyToFixture(f);
//                                }
//                            }
//                            f = f->GetNext();
//                        }
//                        
//                    }
//                    //                else{
//                    //                    [bodySplashes removeObjectForKey:[NSString stringWithFormat:@"%p", b->GetUserData()]];
//                    //                }
//                }
//            }
//        }
//    }
//    
//    
//#else //chipmunk
//    
//    LHPhysicsNode* world = [(LHScene*)[self scene] gameWorldNode];
//    
//    for(CCNode* node in [world children])
//    {
//        CCPhysicsBody* body = [node physicsBody];
//        if(body && [body type] == CCPhysicsBodyTypeDynamic)
//        {
//            CGPoint pos = [node position];
//            
//            float x = pos.x;
//            float y = [self valueAt:[self globalXToWaveX:x]];
//            y = [self waveYToGlobalY:y];
//            
//            if(pos.y < self.position.y + [self contentSize].height*0.5 + y)
//            {
//                
//                NSString* hadSplash = [bodySplashes objectForKey:[NSString stringWithFormat:@"%p", body]];
//                if(hadSplash == nil && splashCollision){
//                    
//                    float splashX = [self globalXToWaveX:x];
//                    
//                    NSArray* splashes = [self createSplash:splashX
//                                                         h:splashHeight*node.scaleY
//                                                         w:splashWidth*node.scaleX
//                                                         t:splashTime];
//                    int i = 0;
//                    for(LHWave* wave in splashes){
//                        [wave step];
//                        ++i;
//                        // if(i > 2)break;
//                    }
//                    [bodySplashes setObject:[NSNumber numberWithBool:YES]
//                                     forKey:[NSString stringWithFormat:@"%p", body]];
//                }
//            }
//            else{
//                [bodySplashes removeObjectForKey:[NSString stringWithFormat:@"%p", body]];
//            }
//            
//        }
//    }
//#endif//LH_USE_BOX2D
    
}







//! a Point with a vertex point and a color 4F
//typedef struct _LH_V3F_C4F
//{
//	//! vertices (3F)
//	ccVertex3F		vertices;
//	//! colors (4F)
//	ccColor4F		colors;
//} _LH_V3F_C4F;
//
////! A Triangle of _LH_V3F_C4F
//typedef struct _LH_V2F_C4B_Triangle
//{
//	//! Point A
//	_LH_V3F_C4F a;
//	//! Point B
//	_LH_V3F_C4F b;
//	//! Point B
//	_LH_V3F_C4F c;
//} _LH_V2F_C4B_Triangle;



//@implementation LHWater
//{
//    LHNodeProtocolImpl* _nodeProtocolImp;
//        
//    float width, height;
//    float numLines;
//    NSMutableArray* waves;
//    float lineWidth;
//
//    float waterDensity;
//    BOOL turbulence;
//    float turbulenceL;
//    float turbulenceH;
//    float turbulenceV;
//    float turbulenceVelocity;
//    
//    BOOL splashCollision;
//    float splashHeight;
//    float splashWidth;
//    float splashTime;
//    NSMutableDictionary* bodySplashes;
//
//    BOOL test;
//    
//    #if LH_USE_BOX2D
//    LH_b2BuoyancyController* buoyancyController;
//    #endif
//}
//
//-(void)dealloc{
//    
//    LH_SAFE_RELEASE(_nodeProtocolImp);
//    
//    LH_SAFE_RELEASE(bodySplashes);
//    LH_SAFE_RELEASE(waves);
//    
//#if LH_USE_BOX2D
//    LH_SAFE_DELETE(buoyancyController);
//#endif
//    
//    LH_SUPER_DEALLOC();
//}


//+ (instancetype)waterNodeWithDictionary:(NSDictionary*)dict
//                                 parent:(CCNode*)prnt{
//    return LH_AUTORELEASED([[self alloc] initWaterNodeWithDictionary:dict
//                                                               parent:prnt]);
//}

/*
- (instancetype)initWaterNodeWithDictionary:(NSDictionary*)dict
                                     parent:(CCNode*)prnt{
    
    
    if(self = [super init]){
        
        [prnt addChild:self];
        
#if LH_USE_BOX2D
        buoyancyController = NULL;
#endif
        
        _nodeProtocolImp = [[LHNodeProtocolImpl alloc] initNodeProtocolImpWithDictionary:dict
                                                                                    node:self];
        
        width = [dict floatForKey:@"width"];
        height = [dict floatForKey:@"height"];
        numLines = [dict floatForKey:@"numLines"];
        waves = [[NSMutableArray alloc] init];
        waterDensity = [dict floatForKey:@"waterDensity"];
        lineWidth = width / numLines;
        
        turbulence = [dict boolForKey:@"turbulence"];
        turbulenceL= [dict floatForKey:@"turbulenceL"];
        turbulenceH= [dict floatForKey:@"turbulenceH"];
        turbulenceV= [dict floatForKey:@"turbulenceV"];
        turbulenceVelocity = [dict floatForKey:@"turbulenceVelocity"];
        
        splashCollision = [dict boolForKey:@"splash"];
        splashHeight    = [dict floatForKey:@"splashH"];
        splashWidth     = [dict floatForKey:@"splashW"];
        splashTime      = [dict floatForKey:@"splashT"];
        
        bodySplashes = [[NSMutableDictionary alloc] init];
        
        CGPoint unitPos = [dict pointForKey:@"generalPosition"];
        CGPoint pos = [LHUtils positionForNode:self
                                      fromUnit:unitPos];
        
        NSDictionary* devPositions = [dict objectForKey:@"devicePositions"];
        if(devPositions)
        {
            
#if TARGET_OS_IPHONE
            NSString* unitPosStr = [LHUtils devicePosition:devPositions
                                                   forSize:LH_SCREEN_RESOLUTION];
#else
            LHScene* scene = (LHScene*)[self scene];
            NSString* unitPosStr = [LHUtils devicePosition:devPositions
                                                   forSize:scene.size];
#endif
            
            if(unitPosStr){
                CGPoint unitPos = LHPointFromString(unitPosStr);
                pos = [LHUtils positionForNode:self
                                      fromUnit:unitPos];
            }
        }
        
        [self setPosition:pos];

        
        _shaderProgram = [[CCShaderCache sharedShaderCache] programForKey:kCCShader_PositionColor];
        
        [self setColor:[dict colorForKey:@"colorOverlay"]];
                
        NSArray* childrenInfo = [dict objectForKey:@"children"];
        if(childrenInfo)
        {
            for(NSDictionary* childInfo in childrenInfo)
            {
                CCNode* node = [LHScene createLHNodeWithDictionary:childInfo
                                                            parent:self];
                #pragma unused (node)
            }
        }
        
        
        [self createTurbulence];
    }
    
    return self;
}
 */




//-(CGRect)waveRect{
//    
//    CGPoint pos = [self convertToWorldSpaceAR:CGPointZero];
//    
////    CGSize size = self.contentSize;
//    return CGRectMake(pos.x - width* self.scaleX *0.5,
//                      pos.y - height* self.scaleY *0.5,
//                      width * self.scaleX,
//                      height* self.scaleY);
//}
//
//
//-(void)ensureCapacity:(NSUInteger)count
//{
//	if(_bufferCount + count > _bufferCapacity){
//		_bufferCapacity += MAX(_bufferCapacity, count);
//		_buffer = (ccV2F_C4B_T2F*)realloc(_buffer, _bufferCapacity*sizeof(ccV2F_C4B_T2F));
//	}
//}
//
//-(void)render
//{
//	if( _dirty ) {
//		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)*_bufferCapacity, _buffer, GL_STREAM_DRAW);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		_dirty = NO;
//	}
//	
//	ccGLBindVAO(_vao);
//	glDrawArrays(GL_TRIANGLES, 0, _bufferCount);
//	
//	CC_INCREMENT_GL_DRAWS(1);
//	
//	CHECK_GL_ERROR();
//}
//
//-(void)draw
//{
//	ccGLBlendFunc(_blendFunc.src, _blendFunc.dst);
//    
//    [_shaderProgram use];
//	[_shaderProgram setUniformsForBuiltins];
//	
//	[self render];
//}
//#if LH_USE_BOX2D
//-(CGRect)boundingRectForBody:(b2Body*)bd
//{
//    b2AABB aabb;
//    b2Transform t;
//    t.SetIdentity();
//    aabb.lowerBound = b2Vec2(FLT_MAX,FLT_MAX);
//    aabb.upperBound = b2Vec2(-FLT_MAX,-FLT_MAX);
//    b2Fixture* fixture = bd->GetFixtureList();
//    while (fixture != NULL) {
//        const b2Shape *shape = fixture->GetShape();
//        const int childCount = shape->GetChildCount();
//        for (int child = 0; child < childCount; ++child) {
//            const b2Vec2 r(shape->m_radius, shape->m_radius);
//            b2AABB shapeAABB;
//            shape->ComputeAABB(&shapeAABB, t, child);
//            shapeAABB.lowerBound = shapeAABB.lowerBound + r;
//            shapeAABB.upperBound = shapeAABB.upperBound - r;
//            aabb.Combine(shapeAABB);
//        }
//        fixture = fixture->GetNext();
//    }
//    
//    LHScene* scene = [self scene];
//    
//    float wm = aabb.upperBound.x - aabb.lowerBound.x;
//    float hm = aabb.upperBound.y - aabb.lowerBound.y;
//    float x = [scene valueFromMeters:aabb.upperBound.x + bd->GetPosition().x - wm];
//    float y = [scene valueFromMeters:aabb.upperBound.y + bd->GetPosition().y - hm];
//    float w = [scene valueFromMeters:wm];
//    float h = [scene valueFromMeters:hm];
//    
//    return CGRectMake(x, y, w, h);
//}
//#endif
//
//
//-(void)visit
//{
//    NSMutableArray* toRemove = [NSMutableArray array];
//    for(LHWave* w in waves){
//        [w step];
//        
//        BOOL done = [w decayBlock] != nil && (w.currentStep >= w.totalSteps);
//        BOOL outBounds = (w.right <= 0 && w.velocity <= 0) || (w.left >= width && w.velocity >= 0);
//        if(done || outBounds) {
//            [toRemove addObject:w];
//        }
//    }
//    [waves removeObjectsInArray:toRemove];
//    
//
//    NSMutableArray* points = [NSMutableArray array];
//    for(int i = 0; i <= numLines; ++i) {
//        [points addObject:[NSNumber numberWithFloat:[self valueAt:(i * lineWidth)]]];
//    }
//
//    [self clear];//clear the geometry in the node buffer
//    
//    
//    float ox = -width*0.5;
//    float oy = -height*0.5;
//    
//    if([points count] > 0)
//    {
//        NSMutableArray* trianglePts = [NSMutableArray array];
//
//        float firstX = -width*0.5;
//        float firstY = height*0.5;
//        NSValue* first = LHValueWithCGPoint(CGPointMake(firstX, firstY));
//        [trianglePts addObject:first];
//        
//        NSValue* prevDown = first;
//        
//        float x = -width*0.5;
//        float y = [[points objectAtIndex:0] floatValue] + oy;
//        [trianglePts addObject:LHValueWithCGPoint(CGPointMake(x, y))];
//        
//        NSValue* prev = nil;
//        for(int i = 1; i < (int)[points count]; ++i)
//        {
//            x = lineWidth*i - width*0.5;
//            y = [[points objectAtIndex:i] floatValue] - height*0.5;
//            
//            if(prev){
//                [trianglePts addObject:first];
//                [trianglePts addObject:prev];
//            }
//            NSValue* curValue = LHValueWithCGPoint(CGPointMake(x, y));
//            [trianglePts addObject:curValue];
//
//            [trianglePts addObject:curValue];
//            
//            
//            NSValue* down = LHValueWithCGPoint(CGPointMake(x, firstY));
//            [trianglePts addObject:down];
//            [trianglePts addObject:prevDown];
//            prevDown = down;
//            first = down;
//            
//            prev = curValue;
//        }
//        
//        x = width*0.5;
//        y = height*0.5;
//        [trianglePts addObject:LHValueWithCGPoint(CGPointMake(x, y))];
//        
//        x = width + ox;
//        y = -height*0.5;
//        [trianglePts addObject:LHValueWithCGPoint(CGPointMake(x, y))];
//        [trianglePts addObject:first];
//        
//        
//        [self ensureCapacity:[trianglePts count]];
//        for(int i = 0; i < [trianglePts count]; i+=3)
//        {
//            NSValue* valA = [trianglePts objectAtIndex:i];
//            NSValue* valB = [trianglePts objectAtIndex:i+1];
//            NSValue* valC = [trianglePts objectAtIndex:i+2];
//            
//            CCColor* color = [self color];
//            
//            CGPoint posA =  CGPointFromValue(valA);
//            posA.y = -posA.y;
//
//            
//            CGPoint posB = CGPointFromValue(valB);
//            posB.y = -posB.y;
//
//            
//            CGPoint posC = CGPointFromValue(valC);
//            posC.y = -posC.y;
//            
//            
//            ccV2F_C4B_T2F a = {{posA.x, posA.y}, {color.red*255.0f, color.green*255.0f, color.blue*255.0f, color.alpha*255.0f}};
//            ccV2F_C4B_T2F b = {{posB.x, posB.y}, {color.red*255.0f, color.green*255.0f, color.blue*255.0f, color.alpha*255.0f}};
//            ccV2F_C4B_T2F c = {{posC.x, posC.y}, {color.red*255.0f, color.green*255.0f, color.blue*255.0f, color.alpha*255.0f}};
//            
//            ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
//            triangles[0] = (ccV2F_C4B_T2F_Triangle){a, b, c};
//            
//            _bufferCount += 3;
//        }
//    }
//
//    
//    [super visit];
//
//    
//#if LH_USE_BOX2D
//    
//    LHScene* scene = [self scene];
//    LHPhysicsNode* pNode = [scene gameWorldNode];
//    b2World* world = [pNode box2dWorld];
//    
//    if(NULL == buoyancyController && world){
//        buoyancyController = new LH_b2BuoyancyController(world);
//        buoyancyController->offset = 27;
//    }
//    
//    buoyancyController->density = -waterDensity;
//    
//    if(world){
//        for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()){
//            
//            if(b->GetType() == b2_dynamicBody){
//                
//                CGRect rect = [self waveRect];
//
//                rect.origin.y -= 100;
//                
//                CGRect bodyRect = [self  boundingRectForBody:b];
//                
//                LHNodePhysicsProtocolImp* nodePhy = LH_ID_BRIDGE_CAST(b->GetUserData());
//                CCNode* node = [nodePhy node];
//                
//                if(node){
//                
//                    float scaleX = [node scaleX];
//                    float scaleY = [node scaleY];
//                    
//                    CGPoint bodyPt = [scene pointFromMeters:b->GetPosition()];
//                    
//                    float x = bodyPt.x;
//                    float y = [self valueAt:[self globalXToWaveX:x]];
//                    
//                    y = [self waveYToGlobalY:y];
//                    
//                    rect.origin.y = y - bodyRect.size.height*0.5;
//                    
//                    if(LHRectOverlapsRect(rect,  bodyRect))
//                    {
//                        
//                        BOOL addedSplash = false;
//                        float previousX = -1000000;
//                        b2Fixture* f = b->GetFixtureList();
//                        while (f) {
//                            
//                            if(!f->IsSensor())
//                            {
//                                {
//                                    y = [scene metersFromValue:y];
//
//                                    buoyancyController->offset = y;
//                                    buoyancyController->useDensity = NO;
//                                    buoyancyController->velocity = b2Vec2(0,0);
//                                    if(turbulence){
//                                        float vdirection = 1.0f;
//                                        if(turbulenceV > 0)
//                                            vdirection = -1.0f;
//                                        buoyancyController->velocity = b2Vec2(vdirection*turbulenceVelocity, 0);
//                                    }
//                                    
//                                    NSString* hadSplash = [bodySplashes objectForKey:[NSString stringWithFormat:@"%p", b->GetUserData()]];
//                                    
//                                    bool after = buoyancyController->ApplyToFixture(f);
//                                    if(after && hadSplash == nil && splashCollision){
//                                        
//                                        float splashX = [self globalXToWaveX:x];
//                                        if(previousX != splashX && !addedSplash){
//                                            NSArray* splashes = [self createSplash:splashX h:splashHeight*scaleY w:splashWidth*scaleX t:splashTime];
//                                            previousX = splashX;
//                                            addedSplash = true;
//                                            int i = 0;
//                                            for(LHWave* wave in splashes){
//                                                [wave step];
//                                                ++i;
//                                                // if(i > 2)break;
//                                            }
//                                            
//                                            [bodySplashes setObject:[NSNumber numberWithBool:YES] forKey:[NSString stringWithFormat:@"%p", b->GetUserData()]];
//                                        }
//                                    }
//                                    buoyancyController->ApplyToFixture(f);
//                                }
//                            }
//                            f = f->GetNext();
//                        }
//                        
//                    }
//    //                else{
//    //                    [bodySplashes removeObjectForKey:[NSString stringWithFormat:@"%p", b->GetUserData()]];
//    //                }
//                }
//            }
//        }
//    }
//    
//    
//#else //chipmunk
//    
//    LHPhysicsNode* world = [(LHScene*)[self scene] gameWorldNode];
//
//    for(CCNode* node in [world children])
//    {
//        CCPhysicsBody* body = [node physicsBody];
//        if(body && [body type] == CCPhysicsBodyTypeDynamic)
//        {
//            CGPoint pos = [node position];
//            
//            float x = pos.x;
//            float y = [self valueAt:[self globalXToWaveX:x]];
//            y = [self waveYToGlobalY:y];
//            
//            if(pos.y < self.position.y + [self contentSize].height*0.5 + y)
//            {
//                
//                NSString* hadSplash = [bodySplashes objectForKey:[NSString stringWithFormat:@"%p", body]];
//                if(hadSplash == nil && splashCollision){
//
//                    float splashX = [self globalXToWaveX:x];
//                    
//                    NSArray* splashes = [self createSplash:splashX
//                                                         h:splashHeight*node.scaleY
//                                                         w:splashWidth*node.scaleX
//                                                         t:splashTime];
//                    int i = 0;
//                    for(LHWave* wave in splashes){
//                        [wave step];
//                        ++i;
//                        // if(i > 2)break;
//                    }
//                    [bodySplashes setObject:[NSNumber numberWithBool:YES]
//                                     forKey:[NSString stringWithFormat:@"%p", body]];
//                }
//            }
//            else{
//                [bodySplashes removeObjectForKey:[NSString stringWithFormat:@"%p", body]];
//            }
//
//        }
//    }
//#endif//LH_USE_BOX2D
//
//}

