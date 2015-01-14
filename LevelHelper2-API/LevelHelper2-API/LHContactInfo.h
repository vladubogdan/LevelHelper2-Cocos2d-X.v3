//
//  LHBox2dCollisionHandling.h
//  LevelHelper2-Cocos2d-Xv3
//
//  Created by Bogdan Vladu on 06/07/14.
//  Copyright (c) 2014 VLADU BOGDAN DANIEL PFA. All rights reserved.
//

#ifndef __LEVELHELPER_API_CONTACT_INFO_H__
#define __LEVELHELPER_API_CONTACT_INFO_H__

#include "LHConfig.h"

#if LH_USE_BOX2D

#include "cocos2d.h"

class b2Contact;

using namespace cocos2d;

class LHContactInfo
{
public:
    
    LHContactInfo();
    virtual ~LHContactInfo();
    
    Node*       nodeA;
    Node*       nodeB;
    std::string nodeAShapeName;
    std::string nodeBShapeName;
    int         nodeAShapeID;
    int         nodeBShapeID;
    
    Point       contactPoint;
    float       impulse;
    
    b2Contact*  box2dContact;
};

#endif

#endif //__LEVELHELPER_API_CONTACT_INFO_H__