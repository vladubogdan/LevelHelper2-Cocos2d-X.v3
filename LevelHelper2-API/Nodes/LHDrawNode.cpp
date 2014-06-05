//
//  LHDrawNode.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHDrawNode.h"
#include "LHPointValue.h"

LHDrawNode::LHDrawNode()
{
    _texture = nullptr;
    _glProgram = nullptr;
    _trianglePoints = nullptr;
    _uvPoints = nullptr;
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
        _physicsBody = NULL;
        
        _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
        //setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR));
        _glProgram = cocos2d::ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE)  ;

        clear();
        
        return true;
    }
    return false;
}

void LHDrawNode::clear()
{
    CC_SAFE_DELETE(_trianglePoints);
    CC_SAFE_DELETE(_uvPoints);
}

void LHDrawNode::setShapeTriangles(__Array* triangles, __Array* uvPoints, const Color4F& c4)
{
    this->clear();
    
    _blendFunc.src = GL_SRC_ALPHA;
    _blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    
    _numberOfTriangles = (int)triangles->count();

    _trianglePoints = new cocos2d::Point[_numberOfTriangles*3];
    _uvPoints = new cocos2d::Point[_numberOfTriangles*3];
    
    int p = 0;
    int u = 0;
        
    for(int i = 0; i < triangles->count(); i+=3)
    {
        LHPointValue* valA = (LHPointValue*)triangles->getObjectAtIndex(i);
        LHPointValue* valB = (LHPointValue*)triangles->getObjectAtIndex(i+1);
        LHPointValue* valC = (LHPointValue*)triangles->getObjectAtIndex(i+2);
        
        LHPointValue* uvA = (LHPointValue*)uvPoints->getObjectAtIndex(i);
        LHPointValue* uvB = (LHPointValue*)uvPoints->getObjectAtIndex(i+1);
        LHPointValue* uvC = (LHPointValue*)uvPoints->getObjectAtIndex(i+2);
        
        
        Point pa = valA->getValue();
        Point pb = valB->getValue();
        Point pc = valC->getValue();
        
        Point ua = uvA->getValue();
        Point ub = uvB->getValue();
        Point uc = uvC->getValue();
        
        _trianglePoints[p++] = pa;
        _trianglePoints[p++] = pb;
        _trianglePoints[p++] = pc;
        
        _uvPoints[u++] = ua;
        _uvPoints[u++] = ub;
        _uvPoints[u++] = uc;
    }
}


void LHDrawNode::draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
	GL::bindTexture2D(_texture->getName());
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORDS);
    //VERTEX_ATTRIB_FLAG_TEX_COORDS its named VERTEX_ATTRIB_FLAG_TEX_COORD in 3.1 so we use the value instead (which is the same on all cocos2d-x versions)
    GL::enableVertexAttribs(1 << 0 | 1 << 2);
        
	_glProgram->use();
	
    _glProgram->setUniformsForBuiltins();
	
//    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(cocos2d::Point), _trianglePoints);
//	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, sizeof(cocos2d::Point), _uvPoints);

    //VERTEX_ATTRIB_TEX_COORDS is named VERTEX_ATTRIB_TEX_COORD in 3.1 so we use the value instead (which is the same on all cocos2d-x versions)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(cocos2d::Point), _trianglePoints);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(cocos2d::Point), _uvPoints);
    
	glDrawArrays(GL_TRIANGLES, 0, _numberOfTriangles*3);
}

Texture2D* LHDrawNode::getTexture() const{
    return _texture;
}

void LHDrawNode::setTexture(Texture2D *texture){
    _texture = texture;
}

void LHDrawNode::setBlendFunc(const BlendFunc &blendFunc){
    _blendFunc = blendFunc;
}

const BlendFunc &LHDrawNode::getBlendFunc() const{
    return _blendFunc;
}
