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
    
    static LHCamera* cameraWithDictionary(LHDictionary* dict, Node* prnt);
    
    LHCamera();
    virtual ~LHCamera();
    bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    static  bool isLHCamera(Node* obj){return (0 != dynamic_cast<LHCamera*>(obj));}
    virtual bool isCamera(){return true;}
    
    //for some reason cocos2d-x people decided to make "visit()" method final - so we use this one instead
    virtual void visit(Renderer *renderer, const kmMat4& parentTransform, bool parentTransformUpdated);
    
    
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

    void setPosition(Point position);
private:
    bool _active;
    bool _restricted;
    
    std::string _followedNodeUUID;
    Node* _followedNode;
    
    void resetActiveState();
    void setSceneView();
    Point transformToRestrictivePosition(Point position);
};

#endif //__LEVELHELPER_API_CAMERA_H__