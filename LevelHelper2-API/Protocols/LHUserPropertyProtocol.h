//
//  LHUserPropertyProtocol.h
//  LevelHelper2-Cocos2d-V3
//
//  Created by Bogdan Vladu on 29/04/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#ifndef __LEVELHELPER_API_USER_PROPERTY_PROTOCOL_H__
#define __LEVELHELPER_API_USER_PROPERTY_PROTOCOL_H__

#include "cocos2d.h"
#include "LHDictionary.h"
#include "LHArray.h"

/**
 LevelHelper 2 nodes that can have user properties conform to this protocol.
 */

class LHUserPropertyProtocol
{
public:
    
    virtual ~LHUserPropertyProtocol(){}
    LHUserPropertyProtocol():node(NULL){}
    /**
     The designeted initializer for the user property declared class.
     @param node The node on which this class object is created.
     @return A valid class of the required type.
     */
    static void* customClassInstanceWithNode(Node* node);

    virtual std::string getClassName(){return std::string();}

    /**
     The node on which this user declared class was set.
     */
    Node*  getNode(){return node;}
    void setNode(Node* value){node = value;}
    
    virtual void setPropertiesFromDictionary(LHDictionary* dictionary){}
    
private:
    Node* node;
};

#endif //__LEVELHELPER_API_USER_PROPERTY_PROTOCOL_H__