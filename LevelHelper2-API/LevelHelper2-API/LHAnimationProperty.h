//
//  LHAnimationProperty.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 22/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_ANIM_PROPERTY_H__
#define __LEVELHELPER_API_ANIM_PROPERTY_H__


#include "cocos2d.h"

USING_NS_CC;

class LHDictionary;
class LHAnimation;
class LHFrame;
class LHNodeAnimationProtocol;

class LHAnimationProperty : public Ref
{
public:
    
    static LHAnimationProperty* createWithDictionary(LHDictionary* dict, LHAnimation* anim);
    LHAnimationProperty();
    virtual ~LHAnimationProperty();
    virtual bool initWithDictionary(LHDictionary* dict, LHAnimation* anim);

    
    
    void addKeyFrame(LHFrame* frm);

    __Array* keyFrames(); //array of LHFrame*

    LHAnimation* animation();

    bool isSubproperty();
    
    LHNodeAnimationProtocol* subpropertyNode();
    void setSubpropertyNode(LHNodeAnimationProtocol* val);
    LHAnimationProperty* subpropertyForUUID(const std::string& nodeUuid);
    
    __Array* allSubproperties();//array of LHAnimationProperty*
    
    virtual LHAnimationProperty* newSubpropertyForNode(LHNodeAnimationProtocol* node);
private:
    
    
    void setParentProperty(LHAnimationProperty* val);
    LHAnimationProperty* parentProperty();
    bool canHaveSubproperties();
    void addSubproperty(LHAnimationProperty* prop);
    void removeSubproperty(LHAnimationProperty* prop);
    
    LHAnimationProperty*    _parentProperty;
    LHNodeAnimationProtocol* _subpropertyNode;
    LHAnimation*            _animation;

    __Array* _frames; //array of LHFrame*
    __Dictionary* subproperties;//dict with LHAnimationProperty* key std::string uuid
};

#endif //__LEVELHELPER_API_ANIM_PROPERTY_H__