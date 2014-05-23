//
//  LHAnimation.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHAnimation.h"

//#import "LHNode.h"
#include "LHSprite.h"
//#import "LHCamera.h"
#include "LHScene.h"
#include "LHDictionary.h"

#include "LHNodeProtocol.h"


#include "LHAnimationProperty.h"
#include "LHFrame.h"

#include "LHPositionProperty.h"
#include "LHChildrenPositionsProperty.h"
#include "LHPositionFrame.h"

#include "LHRotationProperty.h"
#include "LHChildrenRotationsProperty.h"
#include "LHRotationFrame.h"

#include "LHScaleProperty.h"
#include "LHChildrenScalesProperty.h"
#include "LHScaleFrame.h"

#include "LHOpacityProperty.h"
#include "LHChildrenOpacitiesProperty.h"
#include "LHOpacityFrame.h"

#include "LHSpriteFrameProperty.h"
#include "LHSpriteFrame.h"

#include "LHCameraActivateProperty.h"

#include "LHNodeAnimationProtocol.h"


LHAnimation::~LHAnimation()
{
    printf("animation dealloc\n");
    
    _node = NULL;
    _scene = NULL;
    
    CC_SAFE_RELEASE(_properties);
}


LHAnimation* LHAnimation::createWithDictionary(LHDictionary* dict, LHNodeAnimationProtocol* n)
{
    LHAnimation* aProp = new LHAnimation();
    
    if (aProp && aProp->initWithDictionary(dict, n)){
        aProp->autorelease();
    }
    else{
        CC_SAFE_DELETE(aProp);
    }
    return aProp;
}
LHAnimation::LHAnimation():_node(NULL),_properties(NULL),_scene(NULL)
{
    printf("animation constructor\n");
    
}
bool LHAnimation::initWithDictionary(LHDictionary* dict, LHNodeAnimationProtocol* n)
{
    printf("DID CREATE ANIMATION\n");
    
    _node = n;
    
    _repetitions= dict->intForKey("repetitions");
    _totalFrames= dict->floatForKey("totalFrames");
    _name       = dict->stringForKey("name");
    _active     = dict->boolForKey("active");
    _fps        = dict->floatForKey("fps");
    
    _properties = new __Array();
    _properties->init();
    
    LHDictionary* propDictInfo = dict->dictForKey("properties");

    __Array* allKeys = propDictInfo->allKeys();
    
    for(int i = 0; i < allKeys->count(); ++i)
    {
        std::string key = ((__String*)allKeys->getObjectAtIndex(i))->getCString();
        LHDictionary* appInf = propDictInfo->dictForKey(key);
        
        LHAnimationProperty* prop = LHAnimationProperty::createWithDictionary(appInf, this);

        _properties->addObject(prop);
    }
    
    printf("animation is active %d", _active);
    if(_active){
        this->setAnimating(true);
    }
    
    _currentRepetition = -1;
    
    return true;
}



LHNodeAnimationProtocol* LHAnimation::node(){
    return _node;
}

std::string LHAnimation::name(){
    return _name;
}

bool LHAnimation::isActive(){
    return _active;
}

void LHAnimation::setActive(bool val){
    _active = val;
    if(_active){
        _node->setActiveAnimation(this);
    }
    else{
        _node->setActiveAnimation(NULL);
    }
}

float LHAnimation::totalTime(){
    return _totalFrames*(1.0f/_fps);
}


float LHAnimation::currentFrame(){
    return _currentTime/(1.0f/_fps);
}
void LHAnimation::setCurrentFrame(float val){
    updateTimeWithValue((val)*(1.0f/_fps));
}

void LHAnimation::resetOneShotFrames(){
    resetOneShotFramesStartingFromFrameNumber(0);
}

void LHAnimation::resetOneShotFramesStartingFromFrameNumber(int frameNumber){

    for(int i = 0; i < _properties->count(); ++i)
    {
        LHAnimationProperty* prop = (LHAnimationProperty*)_properties->getObjectAtIndex(i);
        if(prop)
        {
            __Array* frames = prop->keyFrames();
            if(frames)
            {
                for(int j= 0; j < frames->count(); ++j)
                {
                    LHFrame* frm = (LHFrame*)frames->getObjectAtIndex(j);
                    if(frm->frameNumber() >= frameNumber){
                        frm->setWasShot(false);
                    }
                }
            }
        }
    }
}
void LHAnimation::setAnimating(bool val){
    resetOneShotFrames();
    _animating = val;
    _currentRepetition = 0;
}
bool LHAnimation::animating(){
    return _animating;
}
void LHAnimation::updateTimeWithDelta(float delta){
    setCurrentTime(currentTime() + delta);
}
void LHAnimation::updateTimeWithValue(float val){
    setCurrentTime(val);
}

int LHAnimation::repetitions(){
    return _repetitions;
}

void LHAnimation::setCurrentTime(float val){
    
    _currentTime = val;
    
    animateNodeToTime(_currentTime);
    
    if(_currentTime > totalTime() && _animating)
    {
        if(_currentRepetition < repetitions() + 1)//dont grow this beyound num of repetitions as
            ++_currentRepetition;
        
        
        if(!didFinishAllRepetitions()){
            _currentTime = 0.0f;
            resetOneShotFrames();
        }
    }
    _previousTime = _currentTime;
}

float LHAnimation::currentTime(){
    return _currentTime;
}
bool LHAnimation::didFinishAllRepetitions(){
    if(repetitions() == 0)
        return false;
    
    if(_animating && _currentRepetition >= repetitions()){
        return true;
    }
    return false;
}

void LHAnimation::animateNodeToTime(float time)
{
    if(didFinishAllRepetitions()){
        return;
    }
    
    if(_node)
    {
        if(time > totalTime()){
            time = totalTime();
        }
     
        for(int i = _properties->count()-1; i >= 0; --i)
        {
            LHAnimationProperty* prop = (LHAnimationProperty*)_properties->getObjectAtIndex(i);


            __Array* allSubs = prop->allSubproperties();
            for(int j = 0; j < allSubs->count(); ++j)
            {
                LHAnimationProperty* subprop = (LHAnimationProperty*)allSubs->getObjectAtIndex(j);
                updateNodeWithAnimationProperty(subprop,time);
            }
            
            updateNodeWithAnimationProperty(prop, time);
        }
    }
}

void LHAnimation::updateNodeWithAnimationProperty(LHAnimationProperty* prop, float time)
{
    __Array* frames = prop->keyFrames();
    
    if(!frames){
        return;
    }
    
    LHFrame* beginFrm = NULL;
    LHFrame* endFrm = NULL;
    
    for(int i = 0; i < frames->count(); ++i)
    {
        LHFrame* frm = (LHFrame*)frames->getObjectAtIndex(i);

        if(frm->frameNumber()*(1.0f/_fps) <= time){
            beginFrm = frm;
        }
        
        if(frm->frameNumber()*(1.0f/_fps) > time){
            endFrm = frm;
            break;//exit for
        }
    }
    
    LHNodeAnimationProtocol* animNode = _node;
    
    if(prop->isSubproperty() && prop->subpropertyNode()){
        printf("set animNode as subproperty node");
        animNode = prop->subpropertyNode();
    }
    

    if(LHChildrenPositionsProperty::isLHChildrenPositionsProperty(prop))
    {
        animateNodeChildrenPositionsToTime(time, beginFrm, endFrm, animNode, prop);
    }
    else if(LHPositionProperty::isLHPositionProperty(prop))
    {
        animateNodePositionToTime(time, beginFrm, endFrm, animNode);
    }
    ////////////////////////////////////////////////////////////////////
    else if(LHChildrenRotationsProperty::isLHChildrenRotationsProperty(prop))
    {
        animateNodeChildrenRotationsToTime(time, beginFrm, endFrm, animNode, prop);
    }
    else if(LHRotationProperty::isLHRotationProperty(prop))
    {
        animateNodeRotationToTime(time, beginFrm, endFrm, animNode);
    }
    
    /*
    else if([prop isKindOfClass:[LHChildrenScalesProperty class]])
    {
        [self animateNodeChildrenScalesToTime:time
                                   beginFrame:beginFrm
                                     endFrame:endFrm
                                         node:animNode
                                     property:prop];
    }
    else if([prop isKindOfClass:[LHScaleProperty class]])
    {
        [self animateNodeScaleToTime:time
                          beginFrame:beginFrm
                            endFrame:endFrm
                                node:animNode];
    }
    else if([prop isKindOfClass:[LHChildrenOpacitiesProperty class]])
    {
        [self animateNodeChildrenOpacitiesToTime:time
                                      beginFrame:beginFrm
                                        endFrame:endFrm
                                            node:animNode
                                        property:prop];
    }
    else if([prop isKindOfClass:[LHOpacityProperty class]])
    {
        [self animateNodeOpacityToTime:time
                            beginFrame:beginFrm
                              endFrame:endFrm
                                  node:animNode];
    }
    else if([prop isKindOfClass:[LHSpriteFrameProperty class]])
    {
        [self animateSpriteFrameChangeWithFrame:beginFrm
                                      forSprite:animNode];
    }
    else if([prop isKindOfClass:[LHCameraActivateProperty class]] && [node isKindOfClass:[LHCamera class]])
    {
        [self animateCameraActivationWithFrame:beginFrm];
    }
 */
}

LHScene* LHAnimation::scene(){
    if(!_scene){
        Node* node = dynamic_cast<Node*>(_node);
        if(!node)return NULL;
        _scene = (LHScene*)node->getScene();
    }
    return _scene;
}

Point LHAnimation::convertFramePosition(Point newPos, Node* animNode)
{
//    if([animNode isKindOfClass:[LHCamera class]]){
//        CGSize winSize = [[self scene] contentSize];
//        return CGPointMake(winSize.width*0.5  - newPos.x,
//                           -winSize.height*0.5 - newPos.y);
//    }
    
    LHScene* scene = this->scene();
    Point offset = scene->getDesignOffset();
    Node* p = animNode->getParent();
    if(LHScene::isLHScene(p))// [p isKindOfClass:[CCPhysicsNode class]])
    {
        newPos.x += offset.x;
        newPos.y += offset.y;

        newPos.y += p->getContentSize().height;
    }
    else{
        Size content = p->getContentSize();
    
        newPos.x += content.width*0.5;
        newPos.y += content.height*0.5;
    }
    
    return newPos;
}

void LHAnimation::animateNodeChildrenPositionsToTime(float time, LHFrame* beginFrm, LHFrame* endFrm, LHNodeAnimationProtocol* _animNode, LHAnimationProperty* prop)
{
    //here we handle positions
    LHPositionFrame* beginFrame = (LHPositionFrame*)beginFrm;
    LHPositionFrame* endFrame   = (LHPositionFrame*)endFrm;

    LHNodeProtocol* animNode = dynamic_cast<LHNodeProtocol*>(_animNode);
    
    if(!animNode)
        return;
    
    __Array* children = animNode->getChildrenOfType<Node*>(NULL);

    if(beginFrame && endFrame)
    {
        double beginTime = beginFrame->frameNumber()*(1.0/_fps);
        double endTime   = endFrame->frameNumber()*(1.0/_fps);
        
        double framesTimeDistance = endTime - beginTime;
        double timeUnit = (time-beginTime)/framesTimeDistance; //a value between 0 and 1

        for(int i = 0; i < children->count(); ++i)
        {
            LHNodeProtocol* child = dynamic_cast<LHNodeProtocol*>(children->getObjectAtIndex(i));
            if(!prop->subpropertyForUUID(child->getUuid()))
            {
                Point beginPosition   = beginFrame->positionForUUID(child->getUuid());
                Point endPosition     = endFrame->positionForUUID(child->getUuid());
                
                //lets calculate the new node position based on the start - end and unit time
                double newX = beginPosition.x + (endPosition.x - beginPosition.x)*timeUnit;
                double newY = beginPosition.y + (endPosition.y - beginPosition.y)*timeUnit;
                
                Point newPos(newX, -newY);
                
                Node* childNode = dynamic_cast<Node*>(child);
                
                if(!childNode)
                    return;
                
                newPos = convertFramePosition(newPos, childNode);
                childNode->setPosition(newPos);
            }
        }
    }
    else if(beginFrame)
    {
        //we only have begin frame so lets set positions based on this frame
        for(int i = 0; i < children->count(); ++i)
        {
            LHNodeProtocol* child = dynamic_cast<LHNodeProtocol*>(children->getObjectAtIndex(i));
            
            if(!prop->subpropertyForUUID(child->getUuid()))
            {
                Point beginPosition = beginFrame->positionForUUID(child->getUuid());
                
                Point newPos(beginPosition.x, -beginPosition.y);
                
                Node* childNode = dynamic_cast<Node*>(child);
                
                if(!childNode)
                    return;
                
                newPos = convertFramePosition(newPos, childNode);
                childNode->setPosition(newPos);
            }
        }
    }
}


void LHAnimation::animateNodePositionToTime(float time, LHFrame* beginFrm, LHFrame* endFrm, LHNodeAnimationProtocol* _animNode)
{
    LHNodeProtocol* animNode = dynamic_cast<LHNodeProtocol*>(_animNode);
    
    if(!animNode)
        return;
    
    //here we handle positions
    LHPositionFrame* beginFrame = (LHPositionFrame*)beginFrm;
    LHPositionFrame* endFrame   = (LHPositionFrame*)endFrm;
    
    if(beginFrame && endFrame)
    {
        double beginTime= beginFrame->frameNumber()*(1.0/_fps);
        double endTime  = endFrame->frameNumber()*(1.0/_fps);
        
        double framesTimeDistance = endTime - beginTime;
        double timeUnit = (time-beginTime)/framesTimeDistance; //a value between 0 and 1
     
        Point beginPosition = beginFrame->positionForUUID(animNode->getUuid());
        Point endPosition   = endFrame->positionForUUID(animNode->getUuid());
        
        //lets calculate the new node position based on the start - end and unit time
        double newX = beginPosition.x + (endPosition.x - beginPosition.x)*timeUnit;
        double newY = beginPosition.y + (endPosition.y - beginPosition.y)*timeUnit;

        Point newPos(newX, -newY);
        Node* node = dynamic_cast<Node*>(animNode);
        
        if(!node)
            return;
        
        newPos = this->convertFramePosition(newPos, node);
        node->setPosition(newPos);
    }
    else if(beginFrame)
    {
        //we only have begin frame so lets set positions based on this frame
        Point beginPosition = beginFrame->positionForUUID(animNode->getUuid());
        
        Point newPos = Point(beginPosition.x, -beginPosition.y);
        
        Node* node = dynamic_cast<Node*>(animNode);
        
        if(!node)
            return;
        
        newPos = this->convertFramePosition(newPos, node);
        node->setPosition(newPos);
    }
}

void LHAnimation::animateNodeChildrenRotationsToTime(float time, LHFrame* beginFrm, LHFrame* endFrm, LHNodeAnimationProtocol* _animNode, LHAnimationProperty* prop)
{
    LHRotationFrame* beginFrame    = (LHRotationFrame*)beginFrm;
    LHRotationFrame* endFrame      = (LHRotationFrame*)endFrm;
    
    LHNodeProtocol* animNode = dynamic_cast<LHNodeProtocol*>(_animNode);
    
    if(!animNode)
        return;
    
    __Array* children = animNode->getChildrenOfType<Node*>(NULL);
    
    if(beginFrame && endFrame)
    {
        float beginTime = beginFrame->frameNumber()*(1.0f/_fps);
        float endTime   = endFrame->frameNumber()*(1.0f/_fps);
        
        float framesTimeDistance = endTime - beginTime;
        float timeUnit = (time-beginTime)/framesTimeDistance; //a value between 0 and 1
        
        
        for(int i = 0; i < children->count(); ++i)
        {
            LHNodeProtocol* child = dynamic_cast<LHNodeProtocol*>(children->getObjectAtIndex(i));
            if(!prop->subpropertyForUUID(child->getUuid()))
            {
                float beginRotation = beginFrame->rotationForUUID(child->getUuid());
                float endRotation   = endFrame->rotationForUUID(child->getUuid());
                
                float shortest_angle = fmodf( (fmodf( (endRotation - beginRotation), 360.0f) + 540.0f), 360.0) - 180.0f;
                
                //lets calculate the new value based on the start - end and unit time
                float newRotation = beginRotation + shortest_angle*timeUnit;
                
                Node* childNode = dynamic_cast<Node*>(child);
                if(!childNode)
                    return;
                
                childNode->setRotation(newRotation);
            }
        }
    }
    else if(beginFrame)
    {
        for(int i = 0; i < children->count(); ++i)
        {
            LHNodeProtocol* child = dynamic_cast<LHNodeProtocol*>(children->getObjectAtIndex(i));
            if(!prop->subpropertyForUUID(child->getUuid()))
            {
                //we only have begin frame so lets set value based on this frame
                float beginRotation = beginFrame->rotationForUUID(child->getUuid());

                Node* childNode = dynamic_cast<Node*>(child);
                if(!childNode)
                    return;
                childNode->setRotation(beginRotation);
            }
        }
    }
}


void LHAnimation::animateNodeRotationToTime(float time, LHFrame* beginFrm, LHFrame* endFrm, LHNodeAnimationProtocol* _animNode)
{
    LHNodeProtocol* animNode = dynamic_cast<LHNodeProtocol*>(_animNode);
    
    if(!animNode)
        return;

    LHRotationFrame* beginFrame    = (LHRotationFrame*)beginFrm;
    LHRotationFrame* endFrame      = (LHRotationFrame*)endFrm;
    
    if(beginFrame && endFrame)
    {
        float beginTime = beginFrame->frameNumber()*(1.0f/_fps);
        float endTime = endFrame->frameNumber()*(1.0f/_fps);
        
        
        float framesTimeDistance = endTime - beginTime;
        float timeUnit = (time-beginTime)/framesTimeDistance; //a value between 0 and 1
        
        float beginRotation = beginFrame->rotationForUUID(animNode->getUuid());
        float endRotation   = endFrame->rotationForUUID(animNode->getUuid());
        
        float shortest_angle = fmodf( (fmodf( (endRotation - beginRotation), 360.0f) + 540.0f), 360.0) - 180.0f;
        
        //lets calculate the new value based on the start - end and unit time
        float newRotation = beginRotation + shortest_angle*timeUnit;
        
        Node* node = dynamic_cast<Node*>(animNode);
        if(!node)
            return;
        node->setRotation(newRotation);
    }
    else if(beginFrame)
    {
        //we only have begin frame so lets set value based on this frame
        float beginRotation = beginFrame->rotationForUUID(animNode->getUuid());
        
        Node* node = dynamic_cast<Node*>(animNode);
        if(!node)
            return;
        node->setRotation(beginRotation);
    }
}
/*
-(void)animateNodeChildrenScalesToTime:(float)time
                            beginFrame:(LHFrame*)beginFrm
                              endFrame:(LHFrame*)endFrm
                                  node:(id<LHNodeAnimationProtocol, LHNodeProtocol>)animNode
                              property:(LHAnimationProperty*)prop
{
    //here we handle scale
    LHScaleFrame* beginFrame    = (LHScaleFrame*)beginFrm;
    LHScaleFrame* endFrame      = (LHScaleFrame*)endFrm;
    
    NSArray* children = [animNode childrenOfType:[CCNode class]];
    
    if(beginFrame && endFrame)
    {
        float beginTime = [beginFrame frameNumber]*(1.0f/_fps);
        float endTime = [endFrame frameNumber]*(1.0f/_fps);
        
        float framesTimeDistance = endTime - beginTime;
        float timeUnit = (time-beginTime)/framesTimeDistance; //a value between 0 and 1
        
        for(id<LHNodeProtocol, LHNodeAnimationProtocol> child in children){
            if(![prop subpropertyForUUID:[child uuid]])
            {
                CGSize beginScale = [beginFrame scaleForUUID:[child uuid]];
                CGSize endScale = [endFrame scaleForUUID:[child uuid]];
                
                //lets calculate the new node scale based on the start - end and unit time
                float newX = beginScale.width + (endScale.width - beginScale.width)*timeUnit;
                float newY = beginScale.height + (endScale.height - beginScale.height)*timeUnit;

                
                [child setScaleX:newX];
                [child setScaleY:newY];
            }
        }
    }
    else if(beginFrame)
    {
        for(id<LHNodeProtocol, LHNodeAnimationProtocol> child in children){
            if(![prop subpropertyForUUID:[child uuid]])
            {
                CGSize beginScale = [beginFrame scaleForUUID:[child uuid]];
                [child setScaleX:beginScale.width];
                [child setScaleY:beginScale.height];
            }
        }
    }
}

-(void)animateNodeScaleToTime:(float)time
                   beginFrame:(LHFrame*)beginFrm
                     endFrame:(LHFrame*)endFrm
                         node:(id<LHNodeAnimationProtocol, LHNodeProtocol>)animNode
{
    //here we handle scale
    LHScaleFrame* beginFrame    = (LHScaleFrame*)beginFrm;
    LHScaleFrame* endFrame      = (LHScaleFrame*)endFrm;
    
    if(beginFrame && endFrame)
    {
        float beginTime = [beginFrame frameNumber]*(1.0f/_fps);
        float endTime = [endFrame frameNumber]*(1.0f/_fps);
        
        float framesTimeDistance = endTime - beginTime;
        float timeUnit = (time-beginTime)/framesTimeDistance; //a value between 0 and 1
        
        CGSize beginScale = [beginFrame scaleForUUID:[animNode uuid]];
        CGSize endScale = [endFrame scaleForUUID:[animNode uuid]];
        
        //lets calculate the new node scale based on the start - end and unit time
        float newX = beginScale.width + (endScale.width - beginScale.width)*timeUnit;
        float newY = beginScale.height + (endScale.height - beginScale.height)*timeUnit;
        
        [animNode setScaleX:newX];
        [animNode setScaleY:newY];
    }
    else if(beginFrame)
    {
        CGSize beginScale = [beginFrame scaleForUUID:[animNode uuid]];
        [animNode setScaleX:beginScale.width];
        [animNode setScaleY:beginScale.height];
    }
}


-(void)animateNodeChildrenOpacitiesToTime:(float)time
                               beginFrame:(LHFrame*)beginFrm
                                 endFrame:(LHFrame*)endFrm
                                     node:(id<LHNodeAnimationProtocol, LHNodeProtocol>)animNode
                                 property:(LHAnimationProperty*)prop
{
    //here we handle sprites opacity
    LHOpacityFrame* beginFrame    = (LHOpacityFrame*)beginFrm;
    LHOpacityFrame* endFrame      = (LHOpacityFrame*)endFrm;
    
    NSArray* children = [node childrenOfType:[CCNode class]];
    
    if(beginFrame && endFrame)
    {
        float beginTime = [beginFrame frameNumber]*(1.0f/_fps);
        float endTime = [endFrame frameNumber]*(1.0f/_fps);
        
        float framesTimeDistance = endTime - beginTime;
        float timeUnit = (time-beginTime)/framesTimeDistance; //a value between 0 and 1
        
        for(id<LHNodeProtocol, LHNodeAnimationProtocol> child in children){
            if(![prop subpropertyForUUID:[child uuid]])
            {
                float beginValue = [beginFrame opacityForUUID:[child uuid]];
                float endValue = [endFrame opacityForUUID:[child uuid]];
                
                //lets calculate the new value based on the start - end and unit time
                float newValue = beginValue + (endValue - beginValue)*timeUnit;
                
                [child setOpacity:newValue/255.0f];
            }
        }
    }
    else if(beginFrame)
    {
        for(id<LHNodeProtocol, LHNodeAnimationProtocol> child in children){
            if(![prop subpropertyForUUID:[child uuid]])
            {
                //we only have begin frame so lets set value based on this frame
                float beginValue = [beginFrame opacityForUUID:[child uuid]];
                [child setOpacity:beginValue/255.0f];
            }
        }
    }
}


-(void)animateNodeOpacityToTime:(float)time
                     beginFrame:(LHFrame*)beginFrm
                       endFrame:(LHFrame*)endFrm
                           node:(id<LHNodeAnimationProtocol, LHNodeProtocol>)animNode
{
    //here we handle sprites opacity
    LHOpacityFrame* beginFrame    = (LHOpacityFrame*)beginFrm;
    LHOpacityFrame* endFrame      = (LHOpacityFrame*)endFrm;
    
    if(beginFrame && endFrame)
    {
        float beginTime = [beginFrame frameNumber]*(1.0f/_fps);
        float endTime = [endFrame frameNumber]*(1.0f/_fps);
        
        float framesTimeDistance = endTime - beginTime;
        float timeUnit = (time-beginTime)/framesTimeDistance; //a value between 0 and 1
        
        float beginValue = [beginFrame opacityForUUID:[animNode uuid]];
        float endValue = [endFrame opacityForUUID:[animNode uuid]];
        
        //lets calculate the new value based on the start - end and unit time
        float newValue = beginValue + (endValue - beginValue)*timeUnit;
                
        [animNode setOpacity:newValue/255.0f];
    }
    else if(beginFrame)
    {
        //we only have begin frame so lets set value based on this frame
        float beginValue = [beginFrame opacityForUUID:[animNode uuid]];
        [animNode setOpacity:beginValue/255.0f];
    }
}

-(void)animateSpriteFrameChangeWithFrame:(LHFrame*)beginFrm
                               forSprite:(id<LHNodeAnimationProtocol, LHNodeProtocol>)animNode
{
    LHSprite* sprite = [animNode isKindOfClass:[LHSprite class]] ? (LHSprite*)animNode : nil;
    if(!sprite)return;
    
    LHSpriteFrame* beginFrame = (LHSpriteFrame*)beginFrm;
    if(beginFrame && sprite)
    {
        if(animating)
        {
            if(![beginFrame wasShot])
            {
                [sprite setSpriteFrameWithName:[beginFrame spriteFrameName]];
                [beginFrame setWasShot:YES];
            }
        }
        else{
            [sprite setSpriteFrameWithName:[beginFrame spriteFrameName]];
        }
    }
}

-(void)animateCameraActivationWithFrame:(LHFrame*)beginFrm
{
    LHFrame* beginFrame = (LHFrame*)beginFrm;
    if(beginFrame)
    {
        if(animating)
        {
            if(![beginFrame wasShot])
            {
                [(LHCamera*)node setActive:YES];
                [beginFrame setWasShot:YES];
            }
        }
        else{
            [(LHCamera*)node setActive:YES];
        }
    }
}
*/
