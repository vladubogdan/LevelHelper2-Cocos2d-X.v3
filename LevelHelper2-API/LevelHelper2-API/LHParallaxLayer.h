//
//  LHParallaxLayer.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_PARALLAX_LAYER_H__
#define __LEVELHELPER_API_PARALLAX_LAYER_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHNodeAnimationProtocol.h"

using namespace cocos2d;

/**
 LHParallaxLayer class is used to load a parallax layer object from a level file.
 Users can retrieve node objects by calling the scene (LHScene) childNodeWithName: method.
 */

class LHDictionary;
class LHScene;

class LHParallaxLayer : public Node, public LHNodeProtocol
{
public:
    
    static LHParallaxLayer* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    
    LHParallaxLayer();
    virtual ~LHParallaxLayer();
        
    static  bool isLHParallaxLayer(Node* obj){return (0 != dynamic_cast<LHParallaxLayer*>(obj));}
    virtual bool isParallaxLayer(){return true;}
    
    /**
     Returns the x ratio that is used to calculate the children position.
     */
    float getRatioX(){return _xRatio;}
    
    /**
     Returns the y ratio that is used to calculate the children position.
     */
    float getRatioY(){return _yRatio;}

    Point getInitialPosition(){return initialPosition;}
private:
    float _xRatio;
    float _yRatio;
    Point initialPosition;
};

#endif //__LEVELHELPER_API_PARALLAX_LAYER_H__