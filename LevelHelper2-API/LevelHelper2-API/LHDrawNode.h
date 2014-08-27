//
//  LHDrawNode.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_DRAW_NODE_H__
#define __LEVELHELPER_API_DRAW_NODE_H__

#include "cocos2d.h"

using namespace cocos2d;

class LHDrawNode : public Node, public TextureProtocol
{
public:
    
    static LHDrawNode* create();
    
    LHDrawNode();
    virtual ~LHDrawNode();
    bool init();

    void setShapeTriangles(__Array* triangles, __Array* uvPoints, const Color4F& color);
    void setShapeTriangles(__Array* triangles, __Array* uvPoints, __Array* colors);
    
    virtual Texture2D* getTexture() const;
    virtual void setTexture(Texture2D *texture);
    
    virtual void setBlendFunc(const BlendFunc &blendFunc);
    virtual const BlendFunc &getBlendFunc() const;

#if COCOS2D_VERSION >= 0x00030200
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
#else
    virtual void draw(Renderer *renderer, const Mat4 &transform, bool transformUpdated) override;
#endif
    
    void clear();
    
    void onDraw(const Mat4 &transform, uint32_t flags);
    
private:
    BlendFunc  _blendFunc;            /// It's required for TextureProtocol inheritance
    Texture2D* _texture;
    GLProgram* _glProgram;
    
    cocos2d::Point* _trianglePoints;
    cocos2d::Point* _uvPoints;
    Color4F*        _colors;
    
    int             _numberOfTriangles;
    
    CustomCommand _customCommand;
};


#endif //__LEVELHELPER_API_DRAW_NODE_H__
