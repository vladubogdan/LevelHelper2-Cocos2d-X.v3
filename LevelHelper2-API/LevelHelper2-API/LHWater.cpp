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
#include "LHValue.h"
#include "LHDrawNode.h"
#include "LHConfig.h"

#if LH_USE_BOX2D
//#include "LHb2BuoyancyController.h"
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

LHWater* LHWater::nodeWithDictionary(LHDictionary* dict, Node* prnt)
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
        
        _drawNode = LHDrawNode::create();
        this->addChild(_drawNode);
        
        prnt->addChild(this);
        
        this->loadGenericInfoFromDictionary(dict);
        this->loadTransformationInfoFromDictionary(dict);
        
        //reset size set by node protocol
        this->setContentSize(Size());


        _width      = dict->floatForKey("width");
        _height     = dict->floatForKey("height");
        _numLines   = dict->floatForKey("numLines");
        
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

        
    
        float alpha = dict->floatForKey("alpha");

        _drawNode->clear();
        
        Color3B overlay = dict->colorForKey("colorOverlay");
        colorOverlay = Color4F(overlay.r/255.0f, overlay.g/255.0f, overlay.b/255.0f, alpha/255.0f);

        _drawNode->setOpacity(alpha);
        
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
    
    __Array* shapeTriangles = __Array::create();
    __Array* uvPoints       = __Array::create();
    __Array* colors         = __Array::create();
    
    for(int i = 0; i < triangles->count(); i+=3)
    {
        LHValue* valA = (LHValue*)triangles->getObjectAtIndex(i);
        LHValue* valB = (LHValue*)triangles->getObjectAtIndex(i+1);
        LHValue* valC = (LHValue*)triangles->getObjectAtIndex(i+2);
        
        Point pA = valA->getPoint();
        pA.y = -pA.y;
        
        Point pB = valB->getPoint();
        pB.y = -pB.y;
        
        Point pC = valC->getPoint();
        pC.y = -pC.y;
        
        shapeTriangles->addObject(LHValue::create(pA));
        shapeTriangles->addObject(LHValue::create(pB));
        shapeTriangles->addObject(LHValue::create(pC));
        
        uvPoints->addObject(LHValue::create(Point()));
        uvPoints->addObject(LHValue::create(Point()));
        uvPoints->addObject(LHValue::create(Point()));
    
        colors->addObject(LHValue::create(c4));
        colors->addObject(LHValue::create(c4));
        colors->addObject(LHValue::create(c4));
    }
    
    _drawNode->setShapeTriangles(shapeTriangles, uvPoints, colors);
}


#if COCOS2D_VERSION >= 0x00030200
void LHWater::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
#else
void LHWater::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
#endif
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
        
        LHValue* first = LHValue::create(Point(firstX, firstY));
        trianglePts->addObject(first);
        
        LHValue* prevDown = first;
        
        float x = -_width*0.5;
        float y = points[0] + oy;
        
        trianglePts->addObject(LHValue::create(Point(x, y)));

        LHValue* prev = nullptr;
        
        for(int i = 1; i < (int)points.size(); ++i)
        {
            x = _lineWidth*i - _width*0.5;
            y = points[i] - _height*0.5;
            
            if(prev){
                trianglePts->addObject(first);
                trianglePts->addObject(prev);
            }
            
            LHValue* curValue = LHValue::create(Point(x, y));
            trianglePts->addObject(curValue);
            
            trianglePts->addObject(curValue);
            
            
            LHValue* down = LHValue::create(Point(x, firstY));
            trianglePts->addObject(down);
            trianglePts->addObject(prevDown);
            prevDown = down;
            first = down;
            
            prev = curValue;
        }
        
        x = _width*0.5;
        y = _height*0.5;
        trianglePts->addObject(LHValue::create(Point(x, y)));
        
        x = _width + ox;
        y = -_height*0.5;
        trianglePts->addObject(LHValue::create(Point(x, y)));
        trianglePts->addObject(first);
        
        this->setShapeTriangles(trianglePts, colorOverlay);
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

