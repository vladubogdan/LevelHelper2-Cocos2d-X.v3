#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;


class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    
    void draw(Renderer* renderer, const kmMat4 &transform, bool transformUpdated);
    
    Texture2D* _texture;
    // The points used to draw the openGl triangle strip
    cocos2d::Point* _areaTrianglePoints;
    // The points used by openGl to apply the testure to the triabgle strip
    cocos2d::Point* _texturePoints;
    // The points on the surface. Not actually used in this demo, but would be used to set the surface sprites for example
    std::vector<cocos2d::Point> _points;
    // An offset, used to allow our textrue to scrol with the triangle list
    cocos2d::Point _textureOffset;
    // The number of points in our triangle array (so 2* the number of surface points)
    int _numberOfPoints;
    // The dy and dx that we use to scroll
    cocos2d::Point _scrollSpeed;
    
    // The openGl program object used
    GLProgram* _glProgram;

    
    void calculateTexturePoints();
};

#endif // __HELLOWORLD_SCENE_H__
