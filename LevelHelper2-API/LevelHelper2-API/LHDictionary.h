//
//  LHDictionary.h
//  Cocos2dX
//
//  Created by Bogdan Vladu on 19/05/14.
//
//

#ifndef __LevelHelper2API__LHDictionary__
#define __LevelHelper2API__LHDictionary__

#include "cocos2d.h"


class LHArray;

using namespace cocos2d;

class LHDictionary : public __Dictionary
{
    
public:
    
    LHDictionary* dictForKey(const std::string& key){
        return (LHDictionary*)this->objectForKey(key);
    }
    
    LHArray* arrayForKey(const std::string& key){
        return (LHArray*)this->objectForKey(key);
    }

    std::string stringForKey(const std::string& key){
        return std::string(this->valueForKey(key)->getCString());
    }
    
    float floatForKey(const std::string& key){
        return this->valueForKey(key)->floatValue();
    }
    
    Point pointForKey(const std::string& key){
        __String* obj = (__String*)this->objectForKey(key);
        return PointFromString(obj->getCString());
    }
    
    Color3B colorForKey(const std::string& key){
        Rect rect = RectFromString(this->valueForKey(key)->getCString());
        return Color3B(rect.origin.x*255.0f, rect.origin.y*255.0f, rect.size.width*255.0f);
    }
    
    Size sizeForKey(const std::string& key){
        return SizeFromString(this->valueForKey(key)->getCString());
    }
    
    Rect rectForKey(const std::string& key){
        return RectFromString(this->valueForKey(key)->getCString());
    }
    
    bool boolForKey(const std::string& key){
        return this->valueForKey(key)->boolValue();
    }
    
    int intForKey(const std::string& key){
        return this->valueForKey(key)->intValue();
    }
    
    void printAllKeys(){
        __Array* keys = this->allKeys();
        for(int i = 0; i < keys->count(); ++i){
            __String* obj = (__String*)keys->getObjectAtIndex(i);
            printf("KEY %d %s\n", i, obj->getCString());
        }
    }
};

#endif /* defined(__LevelHelper2API__LHDictionary__) */
