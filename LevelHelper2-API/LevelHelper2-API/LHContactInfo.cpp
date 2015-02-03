//
//  LHBox2dCollisionHandling.cpp
//  LevelHelper2-Cocos2d-X v3
//
//  Created by Bogdan Vladu on 06/07/14.
//  Copyright (c) 2014 VLADU BOGDAN DANIEL PFA. All rights reserved.
//

#include "LHContactInfo.h"

#if LH_USE_BOX2D

#include "Box2d/Box2d.h"

LHContactInfo::LHContactInfo(){
    nodeA = NULL;
    nodeB = NULL;
    box2dContact = NULL;
}
LHContactInfo::~LHContactInfo(){
    nodeA = NULL;
    nodeB = NULL;
    box2dContact = NULL;
}

#endif