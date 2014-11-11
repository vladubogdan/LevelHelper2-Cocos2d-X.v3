//
//  LHShape.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_SHAPE_H__
#define __LEVELHELPER_API_SHAPE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHNodeAnimationProtocol.h"
#include "LHPhysicsProtocol.h"

using namespace cocos2d;

/**
 LHShape class is used to load and display a shape from a level file.
 Users can retrieve a shape objects by calling the scene (LHScene) childNodeWithName: method.
 */

class LHDrawNode;
class LHScene;
class LHDictionary;

class LHShape : public Node, public LHNodeProtocol, public LHNodeAnimationProtocol, public LHPhysicsProtocol
{
public:
    
    static LHShape* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHShape();
    virtual ~LHShape();
    
    
    static  bool isLHShape(Node* obj){return (0 != dynamic_cast<LHShape*>(obj));}
    virtual bool isShape(){return true;}
    
    /**
     Returns the triangle points used in the drawing of the shape and the physical body.
     */
    const std::vector<Point>& trianglePoints() const;
    
    /**
     Returns the outline points of the shape.
     */
    const std::vector<Point>& outlinePoints() const;
    
#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif
    
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
    std::vector<Point> _triangles;
    std::vector<Point> _outline;
    bool _tile;
    Size _tileScale;
    LHDrawNode*        _drawNode;
    
    Size _contentSize;
    
    void loadShapeFromDictionary(LHDictionary* dict, LHScene* scene);
};


#endif //__LEVELHELPER_API_SHAPE_H__
