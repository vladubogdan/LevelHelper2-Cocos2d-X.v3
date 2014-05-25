//
//  LHBezier.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_BEZIER_H__
#define __LEVELHELPER_API_BEZIER_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHNodeAnimationProtocol.h"

using namespace cocos2d;

/**
LHBezier class is used to load and display a bezier from a level file.
Users can retrieve a bezier objects by calling the scene (LHScene) childNodeWithName: method.
*/

class LHDictionary;

class LHBezier : public DrawNode, public LHNodeProtocol, public LHNodeAnimationProtocol
{
public:
    
    static LHBezier* bezierNodeWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHBezier();
    virtual ~LHBezier();
    bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    static  bool isLHBezier(Node* obj){return (0 != dynamic_cast<LHBezier*>(obj));}
    virtual bool isBezier(){return true;}
    
    //for some reason cocos2d-x people decided to make "visit()" method final - so we use this one instead
    virtual void visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated);
    
    /**
     Returns the points used to draw this bezier node. Array of NSValue with CGPoints;
     */
    std::vector<Point> linePoints();

    
private:
    std::vector<Point> _linePoints;
};

#endif // __LEVELHELPER_API_BEZIER_H__
