//
//  LHCamera.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_CAMERA_H__
#define __LEVELHELPER_API_CAMERA_H__


#include "cocos2d.h"
#include "LHNodeProtocol.h"
#include "LHNodeAnimationProtocol.h"

using namespace cocos2d;

/**
 LHCamera class is used to load a camera object from a level file.
 Users can retrieve camera objects by calling the scene (LHScene) cameraWithName: method.
 */

class LHDictionary;
class LHScene;

class LHCamera : public Node, public LHNodeProtocol, public LHNodeAnimationProtocol
{
public:
    
    static LHCamera* nodeWithDictionary(LHDictionary* dict, Node* prnt);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    
    LHCamera();
    virtual ~LHCamera();
    
    static  bool isLHCamera(Node* obj){return (0 != dynamic_cast<LHCamera*>(obj));}
    virtual bool isCamera(){return true;}
    
    
#if COCOS2D_VERSION >= 0x00030200
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
#else
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated);
#endif
    
    /**
     Returns wheter or not this camera is the active camera.
     */
    bool isActive();
    
    /**
     Sets this camera as the active camera.
     @param value A value specifying if this should be the active camera.
     */
    void setActive(bool value);
    
    /**
     Returns the followed node or nil if no node is being fallowed;
     */
    Node* followedNode();
    
    /**
     Set a node that should be followed by this camera.
     @param node The node this camera should follow.
     */
    void followNode(Node* node);
    
    /**
     Returns wheter or not this camera is restricted to the game world rectangle.
     */
    bool restrictedToGameWorld();
    
    /**
     Set the restricted to game world state of this camera.
     @param value Set the restricted state.
     */
    void setRestrictedToGameWorld(bool value);

    /**
     Set the camera position. The center of the view if the camera is active.
     @param position The new camera position.
     */
    void setPosition(Point position);
    
    
    /**
     Set the camera view offset unit. This value is added to the camera position as an offset.
     @param unit The new camera offset unit. This value is multipled with the screen dimensions and added to the camera position.
     */
     void setOffsetUnit(const Point& unit);
    
    /**
     Returns the camera unit offset.
     */
     Point getOffsetUnit();
    
    /**
     Set the important camera view unit. This value is multipled with the screen dimensions. The area is based on the center.
     Based on this area the camera position will be calculated based on the following node movement.
     This value is ignored when camera is not following a node.
     @param unit The new camera important area unit. This value is multipled with the screen dimensions.
     */
     void setImportantAreaUnit(const Size& unit);
    
    /**
     Returns the camera important area unit.
     */
     Size getImportantAreaUnit();
    
    /**
     Set whether or not the camera should move on x axis.
     This value is ignored when camera is not following a node.
     @param val A boolean value specifying if camera should move on x axis.
     */
     void setLockX(bool val);
    
    /**
     Returns the camera x axis movement locking state.
     */
     bool getLockX();
    
    /**
     Set whether or not the camera should move on y axis.
     This value is ignored when camera is not following a node.
     @param val A boolean value specifying if camera should move on y axis.
     */
     void setLockY(bool val);
    
    /**
     Returns the camera y axis movement locking state.
     */
     bool getLockY();
    
    /**
     When an important area is set, and the following node has exist it or has changed direction,
     smooth movement will make the camera reach its new position in a non-snapping mode.
     This value is ignored when camera is not following a node.
     @param val A boolean value specifying if camera should reach its important area smoothly.
     */
     void setSmoothMovement(bool val);
    
    /**
     Returns if the camera is trying to reach the important area smoothly.
     */
     bool getSmoothMovement();
    
    
#pragma mark - ZOOMING
    
    /**
     Set the camera zoom level by adding the value to the current zoom level.
     Only works if the camera is active.
     @param value Set zoom value that will be added/substracted from the currect camera zoom level.
     @param seconds The time needed for the camera to reach the zoom value specified.
     */
     void zoomByValueInSeconds(float value, float seconds);
    
    /**
     Set the camera zoom level.
     Only works if the camera is active.
     @param value Set new zoom value of the camera.
     @param seconds The time needed for the camera to reach the zoom value specified.
     */
     void zoomToValueInSeconds(float value, float seconds);
    
    /**
     Get the current camera zoom value.
     */
     float getZoomValue();
    
    /**
     Set the camera zoom value without any delay.
     @param value Set new zoom value of the camera.
     */
     void setZoomValue(float value);
    
#pragma mark - LOOK AT
    
    /**
     Makes the camera to look at a specific position by moving from the current or followed object position to this new position in a period of time.
     You should not use this method to move the camera manually. Use setPosition: instead.
     This method should be used only when you want to make the player aware of something in the game world, like a checkpoint it needs to reach.
     @param gwPosition The position the camera will look at. A point value in Game World Node coordinate.
     @param seconds The time needed for the camera to reach the position value specified.
     */
     void lookAtPositionInSeconds(const Point& gwPosition, float seconds);
    
    /**
     Makes the camera to look at a specific node by moving from the current or followed object position to the position of the node in a period of time.
     You should not use this method to move the camera manually. Use setPosition: instead.
     This method should be used only when you want to make the player aware of something in the game world, like a checkpoint it needs to reach.
     @param node The node the camera will look at. A CCNode* derived object (e.g LHSprite, LHNode, ...).
     @param seconds The time needed for the camera to reach the position of the node specified.
     */
     void lookAtNodeInSeconds(Node* node, float seconds);
    
    /**
     Resets the lookAt position by moving the camera back to its original position before the camera was made to lookAt or to the followed object position.
     The reset is instant.
     */
     void resetLookAt();
    
    /**
     Resets the lookAt position by moving the camera back to its original position before the camera was made to lookAt or to the followed object position in a period of time.
     @param seconds The time needed for the camera to move back from the lookAt position to its original or followed node position.
     */
     void resetLookAtInSeconds(float seconds);
    
    /**
     Returns whether or not this camera is currently looking at something. A boolean value.;
     */
     bool isLookingAt();
    
#pragma mark - PINCH ZOOM
    /**
     Sets the camera to zoom on pinch gesture on iOS or scroll wheel on Mac OS when active.
     The zoom will be centered on the followed node or on the center of the pinch.
     */
    void setUsePinchOrScrollWheelToZoom(bool value);
    
    /**
     Returns whether or not this camera is zooming on pinch gesture on iOS or on scroll wheel on Mac OS.
     */
     bool usePinchOrScrollWheelToZoom();
        
private:
    
    friend class LHParallax;
    friend class LHScene;
    
    bool _active;
    bool _restricted;
    
    std::string _followedNodeUUID;
    Node* _followedNode;
    
    void resetActiveState();
    void setSceneView();
    Point transformToRestrictivePosition(Point position);
    
    bool _zoomsOnPinch;
    
    bool wasUpdated;
    
    Point offset;
    Size importantArea;
    
    Point previousFollowedPosition;
    Point previousDirectionVector;
    
    Point directionalOffset;
    Point directionalOffsetToReach;
    float directionMultiplierX;
    float directionMultiplierY;
    
    bool reachingOffsetX;
    bool reachingOffsetY;
    
    
    bool lockX;
    bool lockY;
    bool smoothMovement;
    
    bool zooming;
    float startZoomValue;
    float reachZoomValue;
    float reachZoomTime;
    float minZoomValue;
    unsigned long long zoomStartTime;
    
    Point _centerPosition;//camera pos or followed node position (used by resetLookAt)
    Point _viewPosition;//actual camera view position

    bool lookingAt;
    bool resetingLookAt;
    Point lookAtPosition;
    Node* _lookAtNode;
    
    Point startLookAtPosition;
    float lookAtTime;
    unsigned long long lookAtStartTime;
    
    void performVisit();
    void pinchZoomWithScaleDeltaAndCenter(float delta, const Point& scaleCenter);
};

#endif //__LEVELHELPER_API_CAMERA_H__