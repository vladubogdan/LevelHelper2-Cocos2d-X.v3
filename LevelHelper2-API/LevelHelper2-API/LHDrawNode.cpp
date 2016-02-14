//
//  LHDrawNode.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHDrawNode.h"
#include "LHValue.h"

LHDrawNode::LHDrawNode()
{
    _texture = nullptr;
    _glProgram = nullptr;
    _trianglePoints = nullptr;
    _uvPoints = nullptr;
    _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
}

LHDrawNode::~LHDrawNode()
{
    _texture = nullptr;
    _glProgram = nullptr;
    clear();
}

LHDrawNode* LHDrawNode::create()
{
    LHDrawNode *ret = new LHDrawNode();
    if (ret && ret->init())
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

bool LHDrawNode::init()
{
    if(Node::init())
    {
        _physicsBody = nullptr;
        _trianglePoints = nullptr;
        _uvPoints = nullptr;
        _colors = nullptr;
        
        _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
        //_glProgram = ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR);

        _glProgram = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR);
        
        
        clear();
        
        return true;
    }
    return false;
}

void LHDrawNode::clear()
{
    CC_SAFE_DELETE_ARRAY(_trianglePoints);
    CC_SAFE_DELETE_ARRAY(_uvPoints);
    CC_SAFE_DELETE_ARRAY(_colors);
}

void LHDrawNode::setShapeTriangles(__Array* triangles, __Array* uvPoints, const Color4F& c4)
{
    this->clear();
    
    _blendFunc.src = GL_SRC_ALPHA;
    _blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    
    _numberOfTriangles = (int)triangles->count();

    _trianglePoints = new cocos2d::Point[_numberOfTriangles*3];
    _uvPoints       = new cocos2d::Point[_numberOfTriangles*3];
    _colors         = new Color4F[_numberOfTriangles*3];
    
    
    this->setColor(Color3B(c4.r*255.0f, c4.g*255, c4.b*255));
    this->setOpacity(c4.a);
    
    int p = 0;
    int u = 0;
    int c = 0;
        
    for(int i = 0; i < triangles->count(); i+=3)
    {
        LHValue* valA = (LHValue*)triangles->getObjectAtIndex(i);
        LHValue* valB = (LHValue*)triangles->getObjectAtIndex(i+1);
        LHValue* valC = (LHValue*)triangles->getObjectAtIndex(i+2);
        
        LHValue* uvA = (LHValue*)uvPoints->getObjectAtIndex(i);
        LHValue* uvB = (LHValue*)uvPoints->getObjectAtIndex(i+1);
        LHValue* uvC = (LHValue*)uvPoints->getObjectAtIndex(i+2);
        
        
        Point pa = valA->getPoint();
        Point pb = valB->getPoint();
        Point pc = valC->getPoint();
        
        Point ua = uvA->getPoint();
        Point ub = uvB->getPoint();
        Point uc = uvC->getPoint();
        
        _trianglePoints[p++] = pa;
        _trianglePoints[p++] = pb;
        _trianglePoints[p++] = pc;
        
        _uvPoints[u++] = ua;
        _uvPoints[u++] = ub;
        _uvPoints[u++] = uc;
        
        _colors[c++] = c4;
        _colors[c++] = c4;
        _colors[c++] = c4;
    }
}

void LHDrawNode::setShapeTriangles(__Array* triangles, __Array* uvPoints, __Array* colors)
{
    this->clear();
    
    _blendFunc.src = GL_SRC_ALPHA;
    _blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    
    _numberOfTriangles = (int)triangles->count();
    
    _trianglePoints = new cocos2d::Point[_numberOfTriangles*3];
    _uvPoints       = new cocos2d::Point[_numberOfTriangles*3];
    _colors         = new Color4F[_numberOfTriangles*3];
    
    
    this->setColor(Color3B::WHITE);
    this->setOpacity(1);
    
    int p = 0;
    int u = 0;
    int c = 0;
    
    
    for(int i = 0; i < triangles->count(); i+=3)
    {
        LHValue* valA = (LHValue*)triangles->getObjectAtIndex(i);
        LHValue* valB = (LHValue*)triangles->getObjectAtIndex(i+1);
        LHValue* valC = (LHValue*)triangles->getObjectAtIndex(i+2);
        
        LHValue* uvA = (LHValue*)uvPoints->getObjectAtIndex(i);
        LHValue* uvB = (LHValue*)uvPoints->getObjectAtIndex(i+1);
        LHValue* uvC = (LHValue*)uvPoints->getObjectAtIndex(i+2);
        
        LHValue* colorA = (LHValue*)colors->getObjectAtIndex(i);
        LHValue* colorB = (LHValue*)colors->getObjectAtIndex(i+1);
        LHValue* colorC = (LHValue*)colors->getObjectAtIndex(i+2);
        
        Point pa = valA->getPoint();
        Point pb = valB->getPoint();
        Point pc = valC->getPoint();
        
        Point ua = uvA->getPoint();
        Point ub = uvB->getPoint();
        Point uc = uvC->getPoint();
        
        _trianglePoints[p++] = pa;
        _trianglePoints[p++] = pb;
        _trianglePoints[p++] = pc;
        
        _uvPoints[u++] = ua;
        _uvPoints[u++] = ub;
        _uvPoints[u++] = uc;
        
        _colors[c++] = colorA->getColor4F();
        _colors[c++] = colorB->getColor4F();
        _colors[c++] = colorC->getColor4F();
    }
}


#if COCOS2D_VERSION >= 0x00030200
void LHDrawNode::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(LHDrawNode::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
//    onDraw(transform, flags);
}
#else
void LHDrawNode::draw(Renderer *renderer, const Mat4 &transform, bool transformUpdated)
{
    onDraw(transform, 0);
}
#endif


void LHDrawNode::onDraw(const Mat4 &transform, uint32_t flags)
{
    if(_texture)
    {
        GL::blendFunc(_blendFunc.src, _blendFunc.dst);
        GL::bindTexture2D(_texture->getName());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
    }
    else{
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
    }
    
    if(_glProgram){
        _glProgram->use();
        _glProgram->setUniformsForBuiltins(transform);
    }
	
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(cocos2d::Point), _trianglePoints);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(cocos2d::Color4F), _colors);
    
    if(_texture){
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(cocos2d::Point), _uvPoints);
    }
    
	glDrawArrays(GL_TRIANGLES, 0, _numberOfTriangles*3);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,_numberOfTriangles*3);
    CHECK_GL_ERROR_DEBUG();
}

Texture2D* LHDrawNode::getTexture() const{
    return _texture;
}

void LHDrawNode::setTexture(Texture2D *texture){
    _texture = texture;
//    _glProgram = ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR);
    _glProgram = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR);
    
}

void LHDrawNode::setBlendFunc(const BlendFunc &blendFunc){
    _blendFunc = blendFunc;
}

const BlendFunc &LHDrawNode::getBlendFunc() const{
    return _blendFunc;
}
