//
//  LHBezier.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_BEZIER_H__
#define __LEVELHELPER_API_BEZIER_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHNodeAnimationProtocol.h"
#include "LHPhysicsProtocol.h"
#include "LHConfig.h"

using namespace cocos2d;

/**
LHBezier class is used to load and display a bezier from a level file.
Users can retrieve a bezier objects by calling the scene (LHScene) childNodeWithName: method.
*/

class LHDictionary;

class LHBezier : public DrawNode, public LHNodeProtocol, public LHNodeAnimationProtocol, public LHPhysicsProtocol
{
public:
    
    static LHBezier* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    
    LHBezier();
    virtual ~LHBezier();
    
    
    static  bool isLHBezier(Node* obj){return (0 != dynamic_cast<LHBezier*>(obj));}
    virtual bool isBezier(){return true;}
    
#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif
    
    /**
     Returns the points used to draw this bezier node. Array of NSValue with CGPoints;
     */
    std::vector<Point> linePoints();

#if LH_USE_BOX2D
    virtual void setPosition(const cocos2d::Vec2 &pos);
    virtual void setRotation(float rotation);
    virtual void setScaleX(float scaleX);
    virtual void setScaleY(float scaleY);
    virtual void setScale(float scaleX, float scaleY);
    virtual void updatePosition(const cocos2d::Vec2 &pos);
    virtual void updateRotation(float rotation);
#endif
    
private:
    std::vector<Point> _linePoints;
    
    void loadShapeFromDictionary(LHDictionary* dict);
};

#endif // __LEVELHELPER_API_BEZIER_H__
