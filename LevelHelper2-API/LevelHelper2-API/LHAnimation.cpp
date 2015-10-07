//
//  LHAnimation.m
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHAnimation.h"

#include "LHNode.h"
#include "LHSprite.h"
#include "LHCamera.h"
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

#include "LHBone.h"
#include "LHBoneNodes.h"
#include "LHBoneFrame.h"
#include "LHRootBoneProperty.h"

#include "LHCameraActivateProperty.h"

#include "LHNodeAnimationProtocol.h"

#include "LHGameWorldNode.h"
#include "LHBackUINode.h"
#include "LHUINode.h"


LHAnimation::~LHAnimation()
{    
    _node = NULL;
    _scene = NULL;
    
    CC_SAFE_DELETE(_properties);
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

    
}
bool LHAnimation::initWithDictionary(LHDictionary* dict, LHNodeAnimationProtocol* n)
{
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
    
    if(_active){
        this->restart();
        this->setAnimating(true);
    }
    
    _currentRepetition = 0;
    _currentTime = 0.0f;
    
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
    _animating = val;
}
bool LHAnimation::animating(){
    return _animating;
}
void LHAnimation::restart(){
    resetOneShotFrames();
    _currentTime = 0;
    _currentRepetition = 0;
}
void LHAnimation::updateTimeWithDelta(float delta){
    if(_animating)
    {
        setCurrentTime(currentTime() + delta);
    }
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
            
            if(this->scene()){
                this->scene()->didFinishedRepetitionOnAnimation(this);
            }
        }
        else{
            _node->setActiveAnimation(NULL);
            if(this->scene()){
                this->scene()->didFinishedPlayingAnimation(this);
            }
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
     
        for(ssize_t i = _properties->count()-1; i >= 0; --i)
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
//        printf("set animNode as subproperty node");
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
    ////////////////////////////////////////////////////////////////////////////
    else if(LHRootBoneProperty::isLHRootBoneProperty(prop))
    {
        animateRootBonesToTime(time, beginFrm, endFrm, animNode);
    }
    ////////////////////////////////////////////////////////////////////////////
    else if(LHChildrenRotationsProperty::isLHChildrenRotationsProperty(prop))
    {
        animateNodeChildrenRotationsToTime(time, beginFrm, endFrm, animNode, prop);
    }
    else if(LHRotationProperty::isLHRotationProperty(prop))
    {
        animateNodeRotationToTime(time, beginFrm, endFrm, animNode);
    }
    ////////////////////////////////////////////////////////////////////////////
    else if(LHChildrenScalesProperty::isLHChildrenScalesProperty(prop))
    {
        animateNodeChildrenScalesToTime(time, beginFrm, endFrm, animNode, prop);
    }
    else if(LHScaleProperty::isLHScaleProperty(prop))
    {
        animateNodeScaleToTime(time, beginFrm, endFrm, animNode);
    }
    ////////////////////////////////////////////////////////////////////////////
    else if(LHChildrenOpacitiesProperty::isLHChildrenOpacitiesProperty(prop))
    {
        animateNodeChildrenOpacitiesToTime(time, beginFrm, endFrm, animNode, prop);
    }
    else if(LHOpacityProperty::isLHOpacityProperty(prop))
    {
        animateNodeOpacityToTime(time, beginFrm, endFrm, animNode);
    }
    else if(LHSpriteFrameProperty::isLHSpriteFrameProperty(prop))
    {
        animateSpriteFrameChangeWithFrame(beginFrm, animNode);
    }
//    else if([prop isKindOfClass:[LHCameraActivateProperty class]] && [node isKindOfClass:[LHCamera class]])
//    {
//        [self animateCameraActivationWithFrame:beginFrm];
//    }
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
    LHScene* scene = this->scene();
    Size winSize = scene->getContentSize();
    Point offset = scene->getDesignOffset();

    LHNodeProtocol* protocol = dynamic_cast<LHNodeProtocol*>(animNode);
    if(protocol && protocol->isCamera())
    {
        return Point(winSize.width*0.5 - newPos.x,
                     -winSize.height*0.5 - newPos.y);
    }
    
    Node* p = animNode->getParent();
    
    if(p == nullptr ||
       p == scene ||
       p == scene->getGameWorldNode() ||
       p == scene->getUINode() ||
       p == scene->getBackUINode())
    {
        newPos.x += offset.x;
        newPos.y += offset.y;

        if(p != nullptr){
            newPos.y += scene->getDesignResolutionSize().height;
        }
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

void LHAnimation::animateRootBonesToTime(float time, LHFrame* beginFrm, LHFrame* endFrm, LHNodeAnimationProtocol* _animNode)
{
    LHBone* rootBone = dynamic_cast<LHBone*>(_animNode);
    if(!rootBone)return;
    
    LHBoneFrame* beginFrame    = (LHBoneFrame*)beginFrm;
    LHBoneFrame* endFrame      = (LHBoneFrame*)endFrm;
    
    
    if(rootBone->isRoot())
    {
        __Array* allBones = rootBone->getChildrenOfType<LHBone*>();
        
        if(beginFrame && endFrame && beginFrm != endFrm)
        {
            double beginTime= beginFrame->frameNumber()*(1.0/_fps);
            double endTime  = endFrame->frameNumber()*(1.0/_fps);
            
            double framesTimeDistance = endTime - beginTime;
            double timeUnit = (time-beginTime)/framesTimeDistance; //a value between 0 and 1
            
            LHBoneFrameInfo* beginFrmInfo = beginFrame->boneFrameInfoForBoneNamed("__rootBone__");
            LHBoneFrameInfo* endFrmInfo = endFrame->boneFrameInfoForBoneNamed("__rootBone__");
            
            if(beginFrmInfo && endFrmInfo)
            {
                float beginRotation = beginFrmInfo->rotation;
                float endRotation = endFrmInfo->rotation;
                
                float shortest_angle = fmodf( (fmodf( (endRotation - beginRotation), 360.0f) + 540.0f), 360.0) - 180.0f;
                //lets calculate the new value based on the start - end and unit time
                float newRotation = beginRotation + shortest_angle*timeUnit;
                
                Point beginPosition = beginFrmInfo->position;
                Point endPosition = endFrmInfo->position;
                
                //lets calculate the new node position based on the start - end and unit time
                double newX = beginPosition.x + (endPosition.x - beginPosition.x)*timeUnit;
                double newY = beginPosition.y + (endPosition.y - beginPosition.y)*timeUnit;
                
                Point newPos(newX, -newY);
                
                newPos = this->convertFramePosition(newPos, rootBone);
                
                rootBone->setRotation(newRotation);
                rootBone->setPosition(newPos);
            }

            for(int i = 0; i < allBones->count(); ++i)
            {
                LHBone* b = (LHBone*)allBones->getObjectAtIndex(i);
            
                beginFrmInfo= beginFrame->boneFrameInfoForBoneNamed(b->getName());
                endFrmInfo  = endFrame->boneFrameInfoForBoneNamed(b->getName());
                
                if(beginFrmInfo && endFrmInfo)
                {
                    
                    float beginRotation= beginFrmInfo->rotation;
                    float endRotation  = endFrmInfo->rotation;
                    
                    float shortest_angle = fmodf( (fmodf( (endRotation - beginRotation), 360.0f) + 540.0f), 360.0) - 180.0f;
                    //lets calculate the new value based on the start - end and unit time
                    float newRotation = beginRotation + shortest_angle*timeUnit;
                    
                    b->setRotation(newRotation);
                    
                    if(!b->getIsRigid())
                    {
                        Point beginPosition= beginFrmInfo->position;
                        Point endPosition  = endFrmInfo->position;
                        
                        //lets calculate the new node position based on the start - end and unit time
                        double newX = beginPosition.x + (endPosition.x - beginPosition.x)*timeUnit;
                        double newY = beginPosition.y + (endPosition.y - beginPosition.y)*timeUnit;
                        
                        Point newPos = Point(newX, -newY);
                        
                        newPos = this->convertFramePosition(newPos, b);
                        
                        b->setPosition(newPos);
                    }
                }
            }
        }
        else if(beginFrame && !endFrame){
            
            LHBoneFrameInfo* beginFrmInfo = beginFrame->boneFrameInfoForBoneNamed("__rootBone__");
            
            if(beginFrmInfo)
            {
                Point beginPosition = beginFrmInfo->position;
                Point newPos = Point(beginPosition.x, -beginPosition.y);
                
                newPos = this->convertFramePosition(newPos, rootBone);

                float beginRot =  beginFrmInfo->rotation;
                
                rootBone->setRotation(beginRot);
                rootBone->setPosition(newPos);
            }
            
            for(int i = 0; i < allBones->count(); ++i)
            {
                LHBone* b = (LHBone*)allBones->getObjectAtIndex(i);
                
                beginFrmInfo = beginFrame->boneFrameInfoForBoneNamed(b->getName());
                
                if(beginFrmInfo)
                {
                    float newRotation = beginFrmInfo->rotation;
                    b->setRotation(newRotation);
                    
                    if(!b->getIsRigid())
                    {
                        Point beginPosition = beginFrmInfo->position;
                        Point newPos = Point(beginPosition.x, -beginPosition.y);
                        
                        newPos = this->convertFramePosition(newPos, b);
                        
                        b->setPosition(newPos);
                    }
                }
            }
        }
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

void LHAnimation::animateNodeChildrenScalesToTime(float time,
                                                  LHFrame* beginFrm,
                                                  LHFrame* endFrm,
                                                  LHNodeAnimationProtocol* _animNode,
                                                  LHAnimationProperty* prop)
{
    LHNodeProtocol* animNode = dynamic_cast<LHNodeProtocol*>(_animNode);
    
    if(!animNode)
        return;
    //here we handle scale
    LHScaleFrame* beginFrame    = (LHScaleFrame*)beginFrm;
    LHScaleFrame* endFrame      = (LHScaleFrame*)endFrm;
    
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
                Size beginScale = beginFrame->scaleForUUID(child->getUuid());
                Size endScale   = endFrame->scaleForUUID(child->getUuid());
                
                //lets calculate the new node scale based on the start - end and unit time
                float newX = beginScale.width + (endScale.width - beginScale.width)*timeUnit;
                float newY = beginScale.height + (endScale.height - beginScale.height)*timeUnit;

                Node* childNode = dynamic_cast<Node*>(child);
                if(!childNode)
                    return;
                
                childNode->setScaleX(newX);
                childNode->setScaleY(newY);
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
                Size beginScale = beginFrame->scaleForUUID(child->getUuid());
                
                Node* childNode = dynamic_cast<Node*>(child);
                if(!childNode)
                    return;
                
                childNode->setScaleX(beginScale.width);
                childNode->setScaleY(beginScale.height);
            }
        }
    }
}

void LHAnimation::animateNodeScaleToTime(float time,
                                         LHFrame* beginFrm,
                                         LHFrame* endFrm,
                                         LHNodeAnimationProtocol* _animNode)
{
    LHNodeProtocol* animNode = dynamic_cast<LHNodeProtocol*>(_animNode);
    
    if(!animNode)
        return;

    //here we handle scale
    LHScaleFrame* beginFrame    = (LHScaleFrame*)beginFrm;
    LHScaleFrame* endFrame      = (LHScaleFrame*)endFrm;
    
    if(beginFrame && endFrame)
    {
        float beginTime = beginFrame->frameNumber()*(1.0f/_fps);
        float endTime   = endFrame->frameNumber()*(1.0f/_fps);
        
        float framesTimeDistance = endTime - beginTime;
        float timeUnit = (time-beginTime)/framesTimeDistance; //a value between 0 and 1
        
        Size beginScale = beginFrame->scaleForUUID(animNode->getUuid());
        Size endScale   = endFrame->scaleForUUID(animNode->getUuid());
        
        //lets calculate the new node scale based on the start - end and unit time
        float newX = beginScale.width + (endScale.width - beginScale.width)*timeUnit;
        float newY = beginScale.height + (endScale.height - beginScale.height)*timeUnit;
        
        Node* node = dynamic_cast<Node*>(animNode);
        if(!node)
            return;
        
        node->setScaleX(newX);
        node->setScaleY(newY);
    }
    else if(beginFrame)
    {
        Size beginScale = beginFrame->scaleForUUID(animNode->getUuid());
        
        Node* node = dynamic_cast<Node*>(animNode);
        if(!node)
            return;
        
        node->setScaleX(beginScale.width);
        node->setScaleY(beginScale.height);
    }
}


void LHAnimation::animateNodeChildrenOpacitiesToTime(float time,
                                                     LHFrame* beginFrm,
                                                     LHFrame* endFrm,
                                                     LHNodeAnimationProtocol* _animNode,
                                                     LHAnimationProperty* prop)
{
    LHNodeProtocol* animNode = dynamic_cast<LHNodeProtocol*>(_animNode);
    
    if(!animNode)
        return;
    
    //here we handle sprites opacity
    LHOpacityFrame* beginFrame    = (LHOpacityFrame*)beginFrm;
    LHOpacityFrame* endFrame      = (LHOpacityFrame*)endFrm;
    
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
                float beginValue = beginFrame->opacityForUUID(child->getUuid());
                float endValue   = endFrame->opacityForUUID(child->getUuid());
                
                //lets calculate the new value based on the start - end and unit time
                float newValue = beginValue + (endValue - beginValue)*timeUnit;
                
                Node* childNode = dynamic_cast<Node*>(child);
                if(!childNode)
                    return;
                
                childNode->setOpacity(newValue);
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
                float beginValue = beginFrame->opacityForUUID(child->getUuid());
                
                Node* childNode = dynamic_cast<Node*>(child);
                if(!childNode)
                    return;

                childNode->setOpacity(beginValue);
            }
        }
    }
}

void LHAnimation::animateNodeOpacityToTime(float time, LHFrame* beginFrm, LHFrame* endFrm, LHNodeAnimationProtocol* _animNode)
{
    LHNodeProtocol* animNode = dynamic_cast<LHNodeProtocol*>(_animNode);
    
    if(!animNode)
        return;
    
    //here we handle sprites opacity
    LHOpacityFrame* beginFrame    = (LHOpacityFrame*)beginFrm;
    LHOpacityFrame* endFrame      = (LHOpacityFrame*)endFrm;
    
    if(beginFrame && endFrame)
    {
        float beginTime = beginFrame->frameNumber()*(1.0f/_fps);
        float endTime   = endFrame->frameNumber()*(1.0f/_fps);
        
        float framesTimeDistance = endTime - beginTime;
        float timeUnit = (time-beginTime)/framesTimeDistance; //a value between 0 and 1
        
        float beginValue = beginFrame->opacityForUUID(animNode->getUuid());
        float endValue  = endFrame->opacityForUUID(animNode->getUuid());
        
        //lets calculate the new value based on the start - end and unit time
        float newValue = beginValue + (endValue - beginValue)*timeUnit;
        
        Node* node = dynamic_cast<Node*>(animNode);
        if(!node)
            return;
        
        node->setOpacity(newValue);
    }
    else if(beginFrame)
    {
        //we only have begin frame so lets set value based on this frame
        float beginValue = beginFrame->opacityForUUID(animNode->getUuid());
        
        Node* node = dynamic_cast<Node*>(animNode);
        if(!node)
            return;

        node->setOpacity(beginValue);
    }
}

void LHAnimation::animateSpriteFrameChangeWithFrame(LHFrame* beginFrm, LHNodeAnimationProtocol* _animNode)
{
    LHSprite* sprite = dynamic_cast<LHSprite*>(_animNode);
    
    if(!sprite)
        return;
    
    LHSpriteFrame* beginFrame = (LHSpriteFrame*)beginFrm;
    if(beginFrame && sprite)
    {
        if(_animating)
        {
            if(!beginFrame->wasShot())
            {
                sprite->setSpriteFrame(beginFrame->spriteFrameName());
                beginFrame->setWasShot(true);
            }
        }
        else{
            sprite->setSpriteFrame(beginFrame->spriteFrameName());
        }
    }
}
/*
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
