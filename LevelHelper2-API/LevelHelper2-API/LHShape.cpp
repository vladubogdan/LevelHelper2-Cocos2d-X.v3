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
#include "LHDrawNode.h"
#include "LHValue.h"
#include "LHConfig.h"


LHShape::LHShape()
{
    _drawNode = NULL;
    _tile = false;
}

LHShape::~LHShape()
{
    this->shouldRemoveBody();
}

LHShape* LHShape::nodeWithDictionary(LHDictionary* dict, Node* prnt)
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
    if(Node::init())
    {
        _physicsBody = NULL;
       
        LHScene* scene = (LHScene*)prnt->getScene();
        
        this->loadGenericInfoFromDictionary(dict);
        
        this->loadShapeFromDictionary(dict, scene);
        
//        this->setScaleX(1);
//        this->setScaleY(1);

#if LH_USE_BOX2D
        prnt->addChild(this);
        this->loadTransformationInfoFromDictionary(dict);
        this->loadPhysicsFromDictionary(dict->dictForKey("nodePhysics"), (LHScene*)prnt->getScene());
#else
        //cocos2d-chipmunk required that the body is loaded before adding the node to the parent
        this->loadPhysicsFromDictionary(dict->dictForKey("nodePhysics"), (LHScene*)prnt->getScene());
        prnt->addChild(this);
        this->loadTransformationInfoFromDictionary(dict);
#endif
        
        this->loadChildrenFromDictionary(dict);
        this->createAnimationsFromDictionary(dict);
        
        return true;
    }
    return false;
}

void LHShape::loadShapeFromDictionary(LHDictionary* dict, LHScene* scene)
{
    _tile = dict->boolForKey("tileTexture");
    _tileScale = dict->sizeForKey("tileScale");

    
    Size size = this->getContentSize();
    float alpha = dict->floatForKey("alpha");
    
    LHDrawNode* shape = LHDrawNode::create();
    this->addChild(shape);
    shape->setPosition(Point(size.width*0.5, size.height*0.5));
    
    _drawNode = shape;
    
    Texture2D* texture = NULL;
    
    if(dict->objectForKey("relativeImagePath"))
    {
        std::string imgRelPath = dict->stringForKey("relativeImagePath");
        
        std::string filename = LHUtils::getLastPathComponent(imgRelPath);
        std::string foldername = LHUtils::removeLastPathComponent(imgRelPath);
        foldername = LHUtils::getLastPathComponent(foldername);
        
        
        std::string imagePath = LHUtils::getImagePathWithFilename(filename,
                                                                  foldername,
                                                                  scene->getCurrentDeviceSuffix());
        
        texture = Director::getInstance()->getTextureCache()->addImage(imagePath);
        
        if(texture){
            if(_tile){
                Texture2D::TexParams texParams = {GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_REPEAT, GL_REPEAT};
                texture->setTexParameters(texParams);
            }
            else
            {
                Texture2D::TexParams texParams = {GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT};
                texture->setTexParameters(texParams);
            }
            _drawNode->setTexture(texture);
        }
    }
    
    
    
    LHArray* triangles = dict->arrayForKey("triangles");
    
    Color3B overlay = dict->colorForKey("colorOverlay");
    
    Color4F c4(overlay.r/255.0f,
               overlay.g/255.0f,
               overlay.b/255.0f,
               alpha/255.0f);
    
    
    LHArray* points = dict->arrayForKey("points");
    
    for(int i = 0; i < points->count(); ++i)
    {
        LHDictionary* dict = points->dictAtIndex(i);
        Point pt = dict->pointForKey("point");
        pt.y = -pt.y;
        _outline.push_back(pt);
    }
    
    __Array* shapeTriangles = __Array::create();
    __Array* uvPoints       = __Array::create();
    __Array* colors         = __Array::create();
    
//    float scaleX = this->getScaleX();
//    float scaleY = this->getScaleY();
    
    Size imageSize;
    if(texture)
        imageSize = Size(texture->getPixelsWide(), texture->getPixelsHigh());

    for(int i = 0; i < triangles->count(); i+=3)
    {
        LHDictionary* dictA = triangles->dictAtIndex(i);
        LHDictionary* dictB = triangles->dictAtIndex(i+1);
        LHDictionary* dictC = triangles->dictAtIndex(i+2);
        
        Color4F c4A;
        Color4F c4B;
        Color4F c4C;
        
        if(!texture){
            c4A = c4;
            c4B = c4;
            c4C = c4;
        }
        else{
            
            float alpha= dictA->floatForKey("alpha");
            Color3B color = dictA->colorForKey("color");
            c4A     = Color4F(color.r/255.0f,
                              color.g/255.0f,
                              color.b/255.0f,
                              alpha/255.0f);
            
            alpha   = dictB->floatForKey("alpha");
            color   = dictB->colorForKey("color");
            c4B     = Color4F(color.r/255.0f,
                              color.g/255.0f,
                              color.b/255.0f,
                              alpha/255.0f);
            
            alpha   = dictC->floatForKey("alpha");
            color   = dictC->colorForKey("color");
            c4C     = Color4F(color.r/255.0f,
                              color.g/255.0f,
                              color.b/255.0f,
                              alpha/255.0f);
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
        
        if(_tile && texture){
            
            uvA.x = (posA.x/imageSize.width)*(_tileScale.width);
            uvA.y = -(posA.y/imageSize.height)*(_tileScale.height);
            
            uvB.x = (posB.x/imageSize.width)*(_tileScale.width);
            uvB.y = -(posB.y/imageSize.height)*(_tileScale.height);
            
            uvC.x = (posC.x/imageSize.width)*(_tileScale.width);
            uvC.y = -(posC.y/imageSize.height)*(_tileScale.height);
        }

        
//        posA.x *= scaleX;
//        posA.y *= scaleY;
//        
//        posB.x *= scaleX;
//        posB.y *= scaleY;
//        
//        posC.x *= scaleX;
//        posC.y *= scaleY;
        
        
        _triangles.push_back(posA);
        _triangles.push_back(posB);
        _triangles.push_back(posC);
        
        
        shapeTriangles->addObject(LHValue::create(posA));
        shapeTriangles->addObject(LHValue::create(posB));
        shapeTriangles->addObject(LHValue::create(posC));
        
        if(!texture){
            uvA = Point();
            uvB = Point();
            uvC = Point();
        }
        
        uvPoints->addObject(LHValue::create(uvA));
        uvPoints->addObject(LHValue::create(uvB));
        uvPoints->addObject(LHValue::create(uvC));

        
        colors->addObject(LHValue::create(c4A));
        colors->addObject(LHValue::create(c4B));
        colors->addObject(LHValue::create(c4C));
    }
    
    _drawNode->setShapeTriangles(shapeTriangles, uvPoints, colors);
}



const std::vector<Point>& LHShape::trianglePoints() const
{
    return _triangles;
}

const std::vector<Point>& LHShape::outlinePoints() const
{
    return _outline;
}

#if COCOS2D_VERSION >= 0x00030200
void LHShape::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
#else
void LHShape::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
#endif
{
    visitPhysicsProtocol();
    visitActiveAnimation();
    
    if(renderer)
    {
#if COCOS2D_VERSION >= 0x00030200
        Node::visit(renderer, parentTransform, parentFlags);
#else
        Node::visit(renderer, parentTransform, parentTransformUpdated);
#endif
    }
}

#if LH_USE_BOX2D

void LHShape::setPosition(const cocos2d::Vec2 &pos){
    Node::setPosition(pos);
    this->updatePhysicsTransform();
    for (auto& child : this->getChildren()){
        child->setPosition(child->getPosition());
    }
}
void LHShape::setRotation(float rotation){
    Node::setRotation(rotation);
    this->updatePhysicsTransform();
    for (auto& child : this->getChildren()){
        child->setRotation(child->getRotation());
    }
}
void LHShape::setScaleX(float scaleX){
    Node::setScaleX(scaleX);
    this->updatePhysicsScale();
    for (auto& child : this->getChildren()){
        child->setScaleX(child->getScaleX());
    }
}
void LHShape::setScaleY(float scaleY){
    Node::setScaleY(scaleY);
    this->updatePhysicsScale();
    for (auto& child : this->getChildren()){
        child->setScaleY(child->getScaleY());
    }
}
void LHShape::setScale(float scaleX, float scaleY){
    Node::setScale(scaleX, scaleY);
    this->updatePhysicsScale();
    for (auto& child : this->getChildren()){
        child->setScaleX(child->getScaleX());
    }
}
void LHShape::updatePosition(const cocos2d::Vec2 &pos){
    Node::setPosition(pos);
}
void LHShape::updateRotation(float rotation){
    Node::setRotation(rotation);
}
#endif
