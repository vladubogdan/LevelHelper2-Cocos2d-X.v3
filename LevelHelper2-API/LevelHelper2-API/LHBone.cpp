//
//  LHBone.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 31/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHBone.h"
#include "LHDictionary.h"
#include "LHScene.h"
#include "LHDevice.h"
#include "LHConfig.h"
#include "LHBoneNodes.h"
#include "NodeTransform.h"

class LHBoneConnection : public Ref
{
    
public:
    static LHBoneConnection* createWithDictionary(LHDictionary* dict, LHBone* prnt)
    {
        LHBoneConnection *ret = new LHBoneConnection();
        if (ret && ret->intWithDictionary(dict, prnt))
        {
            ret->autorelease();
            return ret;
        }
        else
        {
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
    }
    virtual bool intWithDictionary(LHDictionary* dict, LHBone* prnt)
    {
        bone = prnt;
        
        angleDelta = dict->floatForKey("angleDelta");
        positionDelta = dict->pointForKey("positionDelta");
        
        if(dict->objectForKey("nodeName"))
        {
            connectedNodeName = dict->stringForKey("nodeName");
        }

        return true;
    }
    
    LHBoneConnection(){bone = nullptr; connectedNode = nullptr;}
    virtual ~LHBoneConnection(){bone = nullptr; connectedNode = nullptr;}
    
    void updateDeltas(){
        
        if(!connectedNode)return;
        
        
        float boneWorldAngle = LHNodeTransform::convertToWorldAngle(bone->getParent(), bone->getRotation());
        float spriteWorldAngle = LHNodeTransform::convertToWorldAngle(connectedNode->getParent(), connectedNode->getRotation());
        
        angleDelta = spriteWorldAngle - boneWorldAngle;
        Point nodeWorldPos = connectedNode->convertToWorldSpaceAR(Point(0,0));
        
        positionDelta = bone->convertToNodeSpace(nodeWorldPos);
    }
    
    Node* getConnectedNode(){
        if(connectedNode == nullptr && connectedNodeName.length() > 0 && bone != nullptr){
            LHBoneNodes* bnodes = bone->getRootBoneNodes();
            if(bnodes){
                Node* node = bnodes->getChildNodeWithName(connectedNodeName);
                if(node){
                    connectedNode = node;
                    this->updateDeltas();
                }
            }
        }
        return connectedNode;
    }
    
    
    
    LHBone* bone;
    float angleDelta;
    Point positionDelta;
    std::string connectedNodeName;
private:
    
    Node* connectedNode;
};





LHBone::LHBone()
{
    _connections = nullptr;
}

LHBone::~LHBone()
{
    CC_SAFE_RELEASE(_connections);
}

bool LHBone::isRoot(){
    return !LHBone::isLHBone(this->getParent());
}
LHBone* LHBone::getRootBone(){
    if(this->isRoot()){
        return this;
    }

    return ((LHBone*)this->getParent())->getRootBone();
}
LHBoneNodes* LHBone::getRootBoneNodes(){

    LHBone* root = this->getRootBone();
    
    if(root){

        __Array* children = root->getChildrenOfType<LHBoneNodes*>();
        if(children && children->count() > 0){
            return (LHBoneNodes*)children->getObjectAtIndex(0);
        }
        
    }
    return NULL;
}


LHBone* LHBone::nodeWithDictionary(LHDictionary* dict, Node* prnt)
{
    LHBone *ret = new LHBone();
    if (ret && ret->initWithDictionary(dict, prnt))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool LHBone::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    if(Node::init())
    {
        _physicsBody = NULL;
        this->loadGenericInfoFromDictionary(dict);
        
        prnt->addChild(this);
        this->loadTransformationInfoFromDictionary(dict);

        this->loadChildrenFromDictionary(dict);
        this->createAnimationsFromDictionary(dict);
        
        
        _maxAngle = dict->floatForKey("maxAngle");
        _minAngle = dict->floatForKey("minAngle");
        _rigid = dict->boolForKey("rigid");
        
        _connections = new __Array();
        _connections->init();
        
        
        __Array* conInfo = dict->arrayForKey("connections");

        for(int i = 0; i < conInfo->count(); ++i)
        {
            LHDictionary* conDict = (LHDictionary*)conInfo->getObjectAtIndex(i);
            if(conDict){
                LHBoneConnection* con = LHBoneConnection::createWithDictionary(conDict, this);
                _connections->addObject(con);
            }
        }

        
#if LH_DEBUG
        DrawNode* debug = DrawNode::create();
        this->addChild(debug);
        
        debug->drawSegment(Point(this->getContentSize().width*0.5, 0),
                           Point(this->getContentSize().width*0.5, this->getContentSize().height),
                           1, Color4F(1, 0, 0, 1));
        
#endif//LH_DEBUG
        
        return true;
    }
    return false;
}

#if COCOS2D_VERSION >= 0x00030200
void LHBone::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
#else
void LHBone::visit(Renderer *renderer, const Mat4& parentTransform, bool parentTransformUpdated)
#endif
{
    visitActiveAnimation();

    if(renderer)
    {
#if COCOS2D_VERSION >= 0x00030200
        Node::visit(renderer, parentTransform, parentFlags);
#else
        Node::visit(renderer, parentTransform, parentTransformUpdated);
#endif
    }
}

void LHBone::transformConnectedSprites()
{
    if(!_connections)return;
    
    float curWorldAngle = LHNodeTransform::convertToWorldAngle(this->getParent(), this->getRotation());
    Point curWorldPos = this->getParent()->convertToWorldSpace(this->getPosition());
    
    for(int i = 0; i < _connections->count(); ++i)
    {
        LHBoneConnection* con = (LHBoneConnection*)_connections->getObjectAtIndex(i);
        if(con)
        {
            Node* sprite = con->getConnectedNode();
            if(sprite)
            {
                Point unit =  LHNodeTransform::unitForGlobalPosition(sprite, curWorldPos);
                float newSpriteAngle = LHNodeTransform::convertToNodeAngle(sprite->getParent(), curWorldAngle);
                newSpriteAngle += con->angleDelta;
                
                Point prevAnchor = sprite->getAnchorPoint();
                sprite->setAnchorPoint(unit);
                sprite->setRotation(newSpriteAngle);
                sprite->setAnchorPoint(prevAnchor);

                Point posDif = con->positionDelta;
                Point deltaWorldPos = this->convertToWorldSpace(posDif);
                Point newSpritePos = sprite->getParent()->convertToNodeSpace(deltaWorldPos);
                sprite->setPosition(newSpritePos);
            }
        }
    }
    
    auto& children = this->getChildren();
    for( const auto &n : children){
        LHBone* childBone = dynamic_cast<LHBone*>(n);
        if(childBone){
            childBone->transformConnectedSprites();
        }
    }
}

void LHBone::setPosition(const cocos2d::Vec2 &pos){
    Node::setPosition(pos);
    this->transformConnectedSprites();
}

void LHBone::setRotation(float rotation){
    Node::setRotation(rotation);
    this->transformConnectedSprites();
}

