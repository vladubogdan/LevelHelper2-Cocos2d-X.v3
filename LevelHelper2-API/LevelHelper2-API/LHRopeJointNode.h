//
//  LHRopeJointNode.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 27/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//




#ifndef __LEVELHELPER_API_ROPE_JOINT_NODE_H__
#define __LEVELHELPER_API_ROPE_JOINT_NODE_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHJointsProtocol.h"
#include "LHConfig.h"

using namespace cocos2d;

/**
 LHRopeJointNode class is used to load a LevelHelper rope joint.
 The equivalent in Cocos2d/Chipmunk is a distance CCPhysicsJoint object with minimum distance of 0 and maximum equal with the rope length.
 */

class LHDictionary;
class LHDrawNode;

#if LH_USE_BOX2D
class b2RopeJoint;
class b2Body;
#endif

class LHRopeJointNode : public Node, public LHNodeProtocol, public LHJointsProtocol
{
public:
    
    static LHRopeJointNode* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHRopeJointNode();
    virtual ~LHRopeJointNode();
    
    
    static  bool isLHRopeJointNode(Node* obj){return (0 != dynamic_cast<LHRopeJointNode*>(obj));}
    virtual bool isRopeJointNode(){return true;}
    

#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif

    
    /**
     Returns whether or not this rope joint can be cut.
     */
    bool canBeCut();
    
    /**
     If the line described by ptA and ptB intersects with the rope joint, the rope will be cut in two. This method ignores "canBeCut".
     @param ptA The start point of the line used to cut the rope. In scene coordinates.
     @param ptB The end point of the line used to cut the rope. In scene coordinates.
     */
    void cutWithLineFromPointA(const Point& ptA, const Point& ptB);

    virtual bool lateLoading();
    
    virtual void shouldRemoveJoint();
    
private:
    
    float   _thickness;
    int     _segments;
    bool    _canBeCut;
    float   _fadeOutDelay;
    bool    _removeAfterCut;
    
    float   _length;
    
    float   _uRepetitions;
    float   _vRepetitions;
    
    Rect    colorInfo;
    float   alphaValue;
    
    LHDrawNode* ropeShape;

    LHDrawNode* cutAShapeNode;
    LHDrawNode* cutBShapeNode;

#if LH_USE_BOX2D
    
    b2RopeJoint* cutJointA;
    b2RopeJoint* cutJointB;
    b2RopeJoint* cutJointAB; //we use this to disable collision between body A & B if the rope is cut when they are next to each other
    
    b2Body* cutBodyA;
    b2Body* cutBodyB;
    
#else//chipmunk
    
    PhysicsJointLimit* cutJointA;
    PhysicsJointLimit* cutJointB;
    
#endif

    
    
    unsigned long long _cutTimer;
    
    float cutJointALength;
    float cutJointBLength;
    
    void drawRopeShape(LHDrawNode* shape, Point anchorA, Point anchorB, float length, int no_segments);
    
    int gravityDirectionAngle();
    __Array* ropePointsFromPointA(Point a, Point b, float ropeLength, float numOfSegments,bool* flipped);
    __Array* shapePointsFromRopePoints(__Array* rPoints, float thick, bool flipped);
    __Array* thickLinePointsFrom(const Point& start, const Point& end, const float& width);
};

#endif //__LEVELHELPER_API_ROPE_JOINT_NODE_H__
