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
#include "LHNodeAnimationProtocol.h"

using namespace cocos2d;

/**
 LHRopeJointNode class is used to load a LevelHelper rope joint.
 The equivalent in Cocos2d/Chipmunk is a distance CCPhysicsJoint object with minimum distance of 0 and maximum equal with the rope length.
 */

class LHDictionary;
class LHScene;
class LHDrawNode;

class LHRopeJointNode : public Node, public LHNodeProtocol
{
public:
    
    static LHRopeJointNode* ropeJointNodeWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHRopeJointNode();
    virtual ~LHRopeJointNode();
    bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    static  bool isLHRopeJointNode(Node* obj){return (0 != dynamic_cast<LHRopeJointNode*>(obj));}
    virtual bool isRopeJointNode(){return true;}
    
    //for some reason cocos2d-x people decided to make "visit()" method final - so we use this one instead
    virtual void visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated);
    
    
    /**
     Returns the point where the joint is connected by the first body. In scene coordinates.
     */
    Point getAnchorA();
    
    /**
     Returns the point where the joint is connected by the second body. In scene coordinates.
     */
    Point getAnchorB();
    
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
private:
    PhysicsJointLimit* joint;
    
    float   _thickness;
    int     _segments;
    bool    _canBeCut;
    float   _fadeOutDelay;
    bool    _removeAfterCut;
    
    std::string _nodeAUUID;
    std::string _nodeBUUID;
    Point _relativePosA;
    Point _relativePosB;
    float _length;
    
    float _uRepetitions;
    float _vRepetitions;
    
    Node* nodeA;
    Node* nodeB;
    
    Color4F colorInfo;
    
    LHDrawNode* ropeShape;
    
    void drawRopeShape(LHDrawNode* shape, Point anchorA, Point anchorB, float length, int no_segments);
    
    int gravityDirectionAngle();
    __Array* ropePointsFromPointA(Point a, Point b, float ropeLength, float numOfSegments,bool* flipped);
    __Array* shapePointsFromRopePoints(__Array* rPoints, float thick, bool flipped);
    __Array* thickLinePointsFrom(const Point& start, const Point& end, const float& width);
};

#endif //__LEVELHELPER_API_ROPE_JOINT_NODE_H__
