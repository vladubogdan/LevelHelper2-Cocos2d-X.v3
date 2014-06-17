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

using namespace cocos2d;

/**
 LHShape class is used to load and display a shape from a level file.
 Users can retrieve a shape objects by calling the scene (LHScene) childNodeWithName: method.
 Note: While the class cannot yet display textures but only colored shape, I hope that in the future Apple will add this functionality into SpriteKit.
 */

class LHDictionary;

class LHShape : public DrawNode, public LHNodeProtocol, public LHNodeAnimationProtocol, public TextureProtocol
{
public:
    
    static LHShape* shapeNodeWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHShape();
    virtual ~LHShape();
    bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    static  bool isLHShape(Node* obj){return (0 != dynamic_cast<LHShape*>(obj));}
    virtual bool isShape(){return true;}
    
    //for some reason cocos2d-x people decided to make "visit()" method final - so we use this one instead
    //virtual void visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated);
  
    /**
     Returns the triangle points used in the drawing of the shape and the physical body.
     */
    const std::vector<Point>& trianglePoints() const;
    
    /**
     Returns the outline points of the shape.
     */
    const std::vector<Point>& outlinePoints() const;
    
    virtual Texture2D* getTexture() const;
    virtual void setTexture(Texture2D *texture);
    
    virtual void setBlendFunc(const BlendFunc &blendFunc);
    virtual const BlendFunc &getBlendFunc() const;

    
    void textureDraw(const Mat4 &transform, bool transformUpdated);
    virtual void draw(Renderer *renderer, const Mat4 &transform, bool transformUpdated) override;
    
    virtual void drawTriangle(const Point &p1, const Point &p2, const Point &p3,
                              const Color4F &colorA, const Color4F &colorB, const Color4F &colorC,
                              const Point& t1, const Point& t2, const Point& t3);
    
private:
    Texture2D* _texture;
    BlendFunc  _blendFunc;            /// It's required for TextureProtocol inheritance
    
    std::vector<Point> _triangles;
    std::vector<Point> _outline;
    
    GLProgram* _glProgram;
};


#endif //__LEVELHELPER_API_SHAPE_H__
