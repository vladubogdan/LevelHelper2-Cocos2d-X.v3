#import <CoreGraphics/CoreGraphics.h>
#include "HelloWorldScene.h"

USING_NS_CC;


Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	_textureOffset = cocos2d::Point(0,0);
	_numberOfPoints = 200;
	_texture = Director::getInstance()->getTextureCache()->addImage("DEMO_INDIVIDUAL_IMAGES_EXAMPLE/hd/cartoonRope.png");
    
	_areaTrianglePoints = (cocos2d::Point*) malloc(sizeof(cocos2d::Point) * _numberOfPoints);
	_texturePoints = (cocos2d::Point*) malloc(sizeof(cocos2d::Point) * _numberOfPoints);
    
	float x=0;
	float y=400;
	float maxy=800;
	float maxDx = 50;
	float maxDy = 240;
	for (int i=0;i<_numberOfPoints;i+=2)
	{
		cocos2d::Point p = cocos2d::Point(x,y);
        
		_points.push_back(p);
		// Add a point
		_areaTrianglePoints[i] = p;
		// Add another point, vertically below the previous point
		_areaTrianglePoints[i+1] = cocos2d::Point(x, 0);
        
		x+= rand() % (int)maxDx;
		float dy = (rand() % (int)(maxDy * 2)) - maxDy;
		y+=dy;
        
		if (y>maxy) y=maxy;
		if (y<0) y=20;
	}
    
	_glProgram = cocos2d::ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE)  ;
    return true;
}

void HelloWorld::draw(Renderer* renderer, const kmMat4 &transform, bool transformUpdated)
//void HelloWorld::draw()
{
	_scrollSpeed = cocos2d::Point(-0.5,0);
	_textureOffset -= _scrollSpeed;
    
	for (int i = 0; i < _numberOfPoints; i++)
	{
		_areaTrianglePoints[i] += _scrollSpeed;
	}
    
	HelloWorld::calculateTexturePoints();
    
	// Tell OpenGL this is the texture we're using
	GL::bindTexture2D(_texture->getName());
	// wrap in both the S and T directions (that's X and Y for the rest of us!)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Enable the arrays
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORDS);
    
	_glProgram->use();
	// Tell Cocos2D to pass the CCNode's position/scale/rotation matrix to the shader (well, that's what Ray says!)
	_glProgram->setUniformsForBuiltins();
	// Give OpenGl our array of points that we want to fill
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(cocos2d::Point), _areaTrianglePoints);
	// Give OpenGl the array of points in the texture we want to use to fill the above array of points
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, sizeof(cocos2d::Point), _texturePoints);
	// Tell OpenGl to draw the arrays we gave it
	glDrawArrays(GL_TRIANGLE_STRIP, 0, _numberOfPoints);
}

void HelloWorld::calculateTexturePoints()
{
	for (int i = 0; i < _numberOfPoints; i++)
	{
		// Calculate a point based on the areaTriangle point plus the current offset
		cocos2d::Point p = cocos2d::Point(_areaTrianglePoints[i].x + _textureOffset.x, _areaTrianglePoints[i].y + _textureOffset.y);
		// mod it to be within the bounds of the texture
		_texturePoints[i] = p * (1.0f / _texture->getPixelsWide());
		// reverse the y coordinate
		_texturePoints[i].y = 1.0f - _texturePoints[i].y;
	}
}