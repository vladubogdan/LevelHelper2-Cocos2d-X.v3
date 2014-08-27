//
//  LHGravityArea.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_GRAVITY_AREA_H__
#define __LEVELHELPER_API_GRAVITY_AREA_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"

using namespace cocos2d;

/**
 LHGravityArea class is used to load a gravity area object from a level file.
 Users can retrieve node objects by calling the scene (LHScene) childNodeWithName: method.
 */

class LHDictionary;
class LHScene;

class LHGravityArea : public Node, public LHNodeProtocol
{
public:
    
    static LHGravityArea* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHGravityArea();
    virtual ~LHGravityArea();

    
    
    static  bool isLHGravityArea(Node* obj){return (0 != dynamic_cast<LHGravityArea*>(obj));}
    virtual bool isGravityArea(){return true;}
    
    
    
#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif

    
    /**
     Returns whether or not this gravity area is a radial.
     */
    bool isRadial();

    /**
     Returns the direction in which the force is applied.
     */
    Point getDirection();

    /**
     Returns the force of this gravity area.
     */
    float getForce();
    
private:
    bool _radial;
    float _force;
    Point _direction;
    
    Rect globalRect();
};
#endif //__LEVELHELPER_API_GRAVITY_AREA_H__
