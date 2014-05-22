//
//  LHNodeProtocol.cpp
//  LevelHelper2-Cocos2d-X-v3
//
//  Created by Bogdan Vladu on 24/03/14.
//  Copyright (c) 2014 GameDevHelper.com. All rights reserved.
//

#include "LHNodeProtocol.h"
#include "LHDictionary.h"
#include "LHArray.h"

#include "LHUserProperties.h"

LHNodeProtocol::LHNodeProtocol():userProperty(NULL)
{
    
}
LHNodeProtocol::~LHNodeProtocol()
{
    if(userProperty){
        delete userProperty;
        userProperty = NULL;
    }
}

void LHNodeProtocol::setTags(LHArray* _tags_)
{
    tags.clear();
    for(int i = 0; i < _tags_->count(); ++i)
    {
        tags.push_back(_tags_->stringAtIndex(i));
    }
}
    
void LHNodeProtocol::loadUserPropertyWithDictionary(LHDictionary* dict, Node* node)
{
    if(dict->objectForKey("userPropertyInfo"))
    {
        LHDictionary* userPropInfo  = dict->dictForKey("userPropertyInfo");
        std::string userPropClassName = dict->stringForKey("userPropertyName");
        
        LHUserPropertyProtocol* propInstance = LHUserCustomPropertiesManager::customClassInstanceWithName(userPropClassName, node);
        
        if(propInstance)
        {
            propInstance->setPropertiesFromDictionary(userPropInfo);
            userProperty = propInstance;
        }
    }
}

Node* LHNodeProtocol::getChildNodeWithUUID(const std::string& uuid)
{
    return NULL;
}
