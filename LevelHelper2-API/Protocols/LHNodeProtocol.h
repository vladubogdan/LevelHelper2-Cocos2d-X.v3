//
//  LHNodeProtocol.h
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 24/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_NODE_PROTOCOL_H__
#define __LEVELHELPER_API_NODE_PROTOCOL_H__

#include "cocos2d.h"
#include "LHUserPropertyProtocol.h"

#if LH_USE_BOX2D
class b2Body;
#endif

class LHDictionary;
class LHArray;
class LHScene;

using namespace cocos2d;


/**
 Most of the LevelHelper-2 nodes conforms to this protocol.
 */

//class LHNodeAnimationProtocol;

class LHNodeProtocol
{
public:

    LHNodeProtocol();
    virtual ~LHNodeProtocol();
    
    /**
     Returns the unique name of the node.
     */
    virtual std::string getName(){return name;}
    virtual void setName(const std::string& value){name = std::string(value);}
    
    /**
     Returns the unique identifier of the node.
     */
    virtual std::string getUuid();
    virtual void setUuid(std::string value);
    
    /**
     Returns all tag values of the node.
     */
    virtual std::vector<std::string> getTags(){return tags;}
    virtual void setTags(LHArray* _tags_);
    
    /**
     Returns the user property object assigned to this object or nil.
     */
    virtual LHUserPropertyProtocol* getUserProperty(){return userProperty;}
    
    virtual bool lateLoading(){return true;}

    /**
     Returns a node with the specified unique name or nil if that node is not found in the children hierarchy.
     @param name The unique name of the node.
     @return A node or or NULL.
     */
    virtual Node* getChildNodeWithName(const std::string& name);

    /**
     Returns a node with the specified unique identifier or nil if that node is not found in the children hierarchy.
     @param uuid The unique idenfier of the node.
     @return A node or or NULL.
     */
    virtual Node* getChildNodeWithUUID(const std::string& uuid);

    /**
     Returns all children nodes that have the specified tag values.
     @param tagValues An vector containing tag names.
     @param any Specify if all or just one tag value of the node needs to be in common with the passed ones.
     @return An array of nodes.
     */
    virtual __Array* getChildrenWithTags(const std::vector<std::string>& tagValues, bool any=true);
    

    
    /**
     Returns all children nodes that are of specified class type. The type must be compatible with Cocos2d-X __Array type. (must be of Ref type)
     @param type A "Class" type.
     @return An array with all the found nodes of the specified class.
     * @code
     * //this is how you should use this function
     * __Array* children = myObject->getChildrenOfType<Node*>();
     * @endcode
     
     */
    
    template<typename T>
    __Array* getChildrenOfType(T value = nullptr)
    {
        Node* node = dynamic_cast<Node*>(this);
        
        __Array* array = __Array::create();
        
        auto& children = node->getChildren();
        for( const auto &n : children) {
            
            T dynamicNode = dynamic_cast<T>(n);
            if(dynamicNode != NULL){
                array->addObject(dynamicNode);
            }
            
            LHNodeProtocol* nPro = dynamic_cast<LHNodeProtocol*>(n);
            if(nPro){
                __Array* nOfType = nPro->getChildrenOfType(value);
                array->addObjectsFromArray(nOfType);
            }
        }
        
        return array;
    }

    
    virtual bool isScene(){return false;}
    virtual bool isBackUINode(){return false;}
    virtual bool isGameWorldNode(){return false;}
    virtual bool isUINode(){return false;}
    virtual bool isSprite(){return false;}
    virtual bool isAsset(){return false;}
    virtual bool isNode(){return false;}
    virtual bool isBezier(){return false;}
    virtual bool isShape(){return false;}
    virtual bool isJoint(){return false;}
    virtual bool isCamera(){return false;}
    virtual bool isParallax(){return false;}
    virtual bool isParallaxLayer(){return false;}
    virtual bool isRopeJointNode(){return false;}
    virtual bool isWater(){return false;}

    
    
    void loadUserPropertyWithDictionary(LHDictionary* dict);
    void loadGenericInfoFromDictionary(LHDictionary* dict);
    void loadTransformationInfoFromDictionary(LHDictionary* dict);
    void loadPhysicsFromDictionary(LHDictionary* dict, LHScene* scene);

    void loadChildrenFromDictionary(LHDictionary* dict);

    static Point positionForNode(Node* node, Point unitPos);
    static Node* createLHNodeWithDictionary(LHDictionary* childInfo, Node* prnt);
    
    std::string name;
    std::string uuid;
    std::vector<std::string>tags;
    LHUserPropertyProtocol* userProperty;
    
    void visitNodeProtocol();
    
    AffineTransform absoluteTransform();
    
    
    
#if LH_USE_BOX2D

    b2Body* getBox2dBody(){return _body;}
    
    Point previousScale;
    b2Body* _body;
#endif
};

#endif //__LEVELHELPER_API_NODE_PROTOCOL_H__
