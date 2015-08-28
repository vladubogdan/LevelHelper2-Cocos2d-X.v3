//
//  LHWater.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_WATER_H__
#define __LEVELHELPER_API_WATER_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHNodeAnimationProtocol.h"

#if LH_USE_BOX2D
class LH_b2BuoyancyController;
#endif

using namespace cocos2d;

/*
 LHWater class is used to load and display a water area from a level file.
 Users can retrieve node objects by calling the scene (LHScene) getChildNodeWithName("name") method.
 */

class LHDictionary;
class LHScene;
class LHWave;
class LHDrawNode;

class LHWater : public Node, public LHNodeProtocol, public LHNodeAnimationProtocol
{
public:
    
    static LHWater* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHWater();
    virtual ~LHWater();
    
    static  bool isLHWater(Node* obj){return (0 != dynamic_cast<LHWater*>(obj));}
    virtual bool isWater(){return true;}
    
#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif
    
    Rect getGameWorldWaveRect();

private:
    LHDrawNode* _drawNode;
    
    Color4F colorOverlay;
    
    
    float _width;
    float _height;
    float _numLines;
    float _waterDensity;
    float _lineWidth;
    
    bool _turbulence;
    float _turbulenceL;
    float _turbulenceH;
    float _turbulenceV;
    float _turbulenceVelocity;
    
    bool _splashCollision;
    float _splashHeight;
    float _splashWidth;
    float _splashTime;
    
    std::vector<LHWave*> waves;
    
    void setShapeTriangles(__Array* triangles, const Color4F& c4);
    
    void createTurbulence();
    void clearWaves();
    LHWave* createTurbulance(float h, float v, float l);
    void addWave(LHWave* w);
    float valueAt(float pos);
    float globalXToWaveX(float pointX);
    float waveYToGlobalY(float waveY);
    std::vector<LHWave*> createSplash(float pos, float h, float w, float t);
    
    
#if LH_USE_BOX2D
    LH_b2BuoyancyController* buoyancyController;
    Rect getBoundingRectForBody(b2Body* bd);
    
    ValueMap bodySplashes;
//    std::map<std::string, bool> bodySplashes;
#endif
    
};

#endif //__LEVELHELPER_API_WATER_H__
