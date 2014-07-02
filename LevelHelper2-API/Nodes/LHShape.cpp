//
//  LHShape.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHShape.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHDevice.h"
#include "LHUtils.h"

LHShape::LHShape()
{
    _texture = nullptr;
    _glProgram = nullptr;
}

LHShape::~LHShape()
{
    
}

LHShape* LHShape::shapeNodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHShape *ret = new LHShape();
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

bool LHShape::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(DrawNode::init())
    {
        _physicsBody = NULL;
        
        loadGenericInfoFromDictionary(dict);
        
        float alpha = dict->floatForKey("alpha");
        this->setOpacity(alpha);
        this->setRotation(dict->floatForKey("rotation"));
        this->setZOrder(dict->floatForKey("zOrder"));

                
        LHScene* scene = (LHScene*)prnt->getScene();
        
        _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
        setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
    
        
        if(dict->objectForKey("relativeImagePath"))
        {
            std::string imgRelPath = dict->stringForKey("relativeImagePath");

            std::string filename = LHUtils::getLastPathComponent(imgRelPath);
            std::string foldername = LHUtils::removeLastPathComponent(imgRelPath);
            foldername = LHUtils::getLastPathComponent(foldername);
            
            
            std::string imagePath = LHUtils::getImagePathWithFilename(filename,
                                                                      foldername,
                                                                      scene->getCurrentDeviceSuffix());

            
            Texture2D* tex = Director::getInstance()->getTextureCache()->addImage(imagePath);

            if(tex){
                Texture2D::TexParams texParams = {GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT};
                tex->setTexParameters(texParams);
                setTexture(tex);
                _glProgram = ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR);
            }
        }
        
    
        clear();
        
        
        LHArray* triangles = dict->arrayForKey("triangles");

        Color3B overlay = dict->colorForKey("colorOverlay");
        
        Color4F c4(overlay.r*255.0f,
                   overlay.g*255.0f,
                   overlay.b*255.0f,
                   alpha);
        
        
        LHArray* points = dict->arrayForKey("points");
     
        for(int i = 0; i < points->count(); ++i)
        {
            LHDictionary* dict = points->dictAtIndex(i);
            Point pt = dict->pointForKey("point");
            pt.y = -pt.y;
            _outline.push_back(pt);
        }
        
        
        for(int i = 0; i < triangles->count(); i+=3)
        {
            LHDictionary* dictA = triangles->dictAtIndex(i);
            LHDictionary* dictB = triangles->dictAtIndex(i+1);
            LHDictionary* dictC = triangles->dictAtIndex(i+2);
            
            Color4F c4A;
            Color4F c4B;
            Color4F c4C;
            
            if(!_texture){
                c4A = c4;
                c4B = c4;
                c4C = c4;
            }
            else{
                
                float alpha= dictA->floatForKey("alpha");
                Color3B color = dictA->colorForKey("color");
                c4A     = Color4F(color.r*255.0f,
                              color.g*255.0f,
                              color.b*255.0f,
                              alpha*255.0f);
                
                alpha   = dictB->floatForKey("alpha");
                color   = dictB->colorForKey("color");
                c4B     = Color4F(color.r*255.0f,
                                  color.g*255.0f,
                                  color.b*255.0f,
                                  alpha*255.0f);
                
                alpha   = dictC->floatForKey("alpha");
                color   = dictC->colorForKey("color");
                c4C     = Color4F(color.r*255.0f,
                                  color.g*255.0f,
                                  color.b*255.0f,
                                  alpha*255.0f);
            }
            
            Point posA = dictA->pointForKey("point");
            posA.y = -posA.y;
            Point uvA = dictA->pointForKey("uv");
            
            Point posB = dictB->pointForKey("point");
            posB.y = -posB.y;
            Point uvB = dictB->pointForKey("uv");
            
            Point posC = dictC->pointForKey("point");
            posC.y = -posC.y;
            Point uvC = dictC->pointForKey("uv");
            
            _triangles.push_back(posA);
            _triangles.push_back(posB);
            _triangles.push_back(posC);
            
            if(!_texture){
                uvA = Point();
                uvB = Point();
                uvC = Point();
            }

            drawTriangle(posA, posB, posC, c4A, c4B, c4C, uvA, uvB, uvC);
        }
        
        
        
        
        prnt->addChild(this);
        
        
        Point scl = dict->pointForKey("scale");
        this->setScaleX(scl.x);
        this->setScaleY(scl.y);
        
        Point anchor = dict->pointForKey("anchor");
        anchor.y = 1.0f - anchor.y;
        this->setAnchorPoint(anchor);
        
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
        this->setPosition(pos);
        
        //physics body needs to be created before adding this node to the parent
        loadPhysicsFromDictionary(dict->dictForKey("nodePhysics"), (LHScene*)prnt->getScene());
        
        
        LHArray* childrenInfo = dict->arrayForKey("children");
        if(childrenInfo)
        {
            for(int i = 0; i < childrenInfo->count(); ++i)
            {
                LHDictionary* childInfo = childrenInfo->dictAtIndex(i);
                
                Node* node = LHScene::createLHNodeWithDictionary(childInfo, this);
#pragma unused (node)
            }
        }
        
        createAnimationsFromDictionary(dict);
        
        
        return true;
    }
    return false;
}

void LHShape::drawTriangle(const Point &p1,
                           const Point &p2,
                           const Point &p3,
                           const Color4F &colorA,
                           const Color4F &colorB,
                           const Color4F &colorC,
                           const Point& t1,
                           const Point& t2,
                           const Point& t3)
{
    unsigned int vertex_count = 2*3;
    ensureCapacity(vertex_count);
    
    Color4B colA = Color4B(colorA);
    Color4B colB = Color4B(colorB);
    Color4B colC = Color4B(colorC);
    
    V2F_C4B_T2F a = {Vec2(p1.x, p1.y), colA, Tex2F(t1.x, t1.y) };
    V2F_C4B_T2F b = {Vec2(p2.x, p2.y), colB, Tex2F(t2.x, t2.y) };
    V2F_C4B_T2F c = {Vec2(p3.x, p3.y), colC, Tex2F(t3.x, t3.y) };

    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    V2F_C4B_T2F_Triangle triangle = {a, b, c};
    triangles[0] = triangle;
    
    _bufferCount += vertex_count;
    _dirty = true;

}

void LHShape::draw(Renderer *renderer, const Mat4 &transform, bool transformUpdated)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(LHShape::textureDraw, this, transform, transformUpdated);
    renderer->addCommand(&_customCommand);
}

void LHShape::textureDraw(const Mat4 &transform, bool transformUpdated)
{
    if(_glProgram){
        _glProgram->use();
        _glProgram->setUniformsForBuiltins(transform);
    }
    else{
        getGLProgram()->use();
        getGLProgram()->setUniformsForBuiltins(transform);
    }
    
    GL::blendFunc( _blendFunc.src, _blendFunc.dst );
    
    if(_texture)
    {
        GL::bindTexture2D( _texture->getName() );
    }
    
    if (_dirty)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacity, _buffer, GL_STREAM_DRAW);
        _dirty = false;
    }
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(_vao);
    }
    else
    {
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
        
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        // vertex
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        
        // color
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        
        // texcood
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
    }
    
    glDrawArrays(GL_TRIANGLES, 0, _bufferCount);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,_bufferCount);
    CHECK_GL_ERROR_DEBUG();
}


const std::vector<Point>& LHShape::trianglePoints() const
{
    return _triangles;
}

const std::vector<Point>& LHShape::outlinePoints() const
{
    return _outline;
}

Texture2D* LHShape::getTexture() const{
    return _texture;
}

void LHShape::setTexture(Texture2D *texture){
    _texture = texture;
}

void LHShape::setBlendFunc(const BlendFunc &blendFunc){
    _blendFunc = blendFunc;
}

const BlendFunc &LHShape::getBlendFunc() const{
    return _blendFunc;
}

/*
-(void)visit
{
    NSTimeInterval thisTime = [NSDate timeIntervalSinceReferenceDate];
    float dt = thisTime - lastTime;
    
    if(activeAnimation){
        [activeAnimation updateTimeWithDelta:dt];
    }
    
    [super visit];
    
    lastTime = thisTime;
}

*/