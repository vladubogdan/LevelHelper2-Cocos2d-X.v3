//
//  LHAnimation.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//


#ifndef __LEVELHELPER_API_ANIMATION_H__
#define __LEVELHELPER_API_ANIMATION_H__


#include "cocos2d.h"

#include "LHNodeAnimationProtocol.h"

USING_NS_CC;

class LHDictionary;
class LHAnimationProperty;
class LHScene;
class LHFrame;

/**
 The animation object that is used to transform one or multiple nodes during a period of time.
 */

class LHAnimation : public Ref
{
public:
    
    static LHAnimation* createWithDictionary(LHDictionary* dict, LHNodeAnimationProtocol* n);
    LHAnimation();
    virtual ~LHAnimation();
    virtual bool initWithDictionary(LHDictionary* dict, LHNodeAnimationProtocol* n);
    
    /**
     The name of the animation.
     */
    std::string name();

    /**
     Wheter or not this animation is active. The one that is currently played.
     */
    bool isActive();

    /**
     Set this animation as the active one.
     @param active A bool value specifying the active state of the animation.
     */
    void setActive(bool active);

    /**
     The time it takes for the animation to finish a loop.
     */
    float totalTime();

    /**
     Current frame of the animation. As defines in LevelHelper 2 editor.
     */
    float currentFrame();

    /**
     Move the animation to a frame.
     @param value The frame number where the animation should jump to.
     */
    void setCurrentFrame(float value);

    /**
     Set the animations as playing or paused.
     @param animating A BOOL value that will set the animation as playing or paused.
     */
    void setAnimating(bool animating);

    /**
     Wheter or not the animation is currently playing.
     */
    bool animating();

    /**
     Restarts the animation. Will set the time to 0 and reset all repetitions.
     */
    void restart();

    /**
     The number of times this animation will loop. A 0 repetitions meens it will loop undefinately.
     */
    int repetitions();

    /**
     The node on which this animation is assigned.
     */
    LHNodeAnimationProtocol* node();

    /**
     Force the animation to go forward in time by adding the delta value to the current animation time.
     @param delta A value that will be appended to the current animation time.
     */
    void updateTimeWithDelta(float delta);

    float currentTime();
private:
    LHNodeAnimationProtocol*       _node;
    int         _repetitions;
    float       _totalFrames;
    std::string _name;
    bool        _active;
    float       _fps;
    __Array*    _properties;
    int         _currentRepetition;
    float       _currentTime;
    float       _previousTime;
    bool        _animating;
    LHScene*    _scene;
    
    void resetOneShotFrames();
    void resetOneShotFramesStartingFromFrameNumber(int frameNumber);
    void updateTimeWithValue(float val);
    void setCurrentTime(float val);
    void animateNodeToTime(float time);
    bool didFinishAllRepetitions();
    void updateNodeWithAnimationProperty(LHAnimationProperty* prop, float time);
    
    LHScene* scene();
    Point convertFramePosition(Point newPos, Node* animNode);
    
    void animateNodeChildrenPositionsToTime(float time, LHFrame* beginFrm, LHFrame* endFrm, LHNodeAnimationProtocol* animNode, LHAnimationProperty* prop);
    void animateNodePositionToTime(float time, LHFrame* beginFrm, LHFrame* endFrm, LHNodeAnimationProtocol* animNode);
    
    void animateRootBonesToTime(float time, LHFrame* beginFrm, LHFrame* endFrm, LHNodeAnimationProtocol* _animNode);
    
    void animateNodeChildrenRotationsToTime(float time, LHFrame* beginFrm, LHFrame* endFrm, LHNodeAnimationProtocol* animNode, LHAnimationProperty* prop);
    void animateNodeRotationToTime(float time, LHFrame* beginFrm, LHFrame* endFrm, LHNodeAnimationProtocol* animNode);
    void animateNodeChildrenScalesToTime(float time, LHFrame* beginFrm, LHFrame* endFrm, LHNodeAnimationProtocol* animNode, LHAnimationProperty* prop);
    void animateNodeScaleToTime(float time, LHFrame* beginFrm, LHFrame* endFrm, LHNodeAnimationProtocol* animNode);
    void animateNodeChildrenOpacitiesToTime(float time, LHFrame* beginFrm, LHFrame* endFrm, LHNodeAnimationProtocol* animNode, LHAnimationProperty* prop);
    void animateNodeOpacityToTime(float time, LHFrame* beginFrm, LHFrame* endFrm, LHNodeAnimationProtocol* animNode);
    void animateSpriteFrameChangeWithFrame(LHFrame* beginFrm, LHNodeAnimationProtocol* animNode);
};
#endif //__LEVELHELPER_API_ANIMATION_H__
