//
//  LHArray.h
//  Cocos2dX
//
//  Created by Bogdan Vladu on 19/05/14.
//
//

#ifndef __LevelHelper2API__LHArray_h
#define __LevelHelper2API__LHArray_h

#include "cocos2d.h"

class LHDictionary;

using namespace cocos2d;

class LHArray : public __Array
{
public:
    
    LHDictionary* dictAtIndex(unsigned int index)
    {
        return (LHDictionary*)this->getObjectAtIndex(index);
    }
    
    LHArray* arrayAtIndex(unsigned int index)
    {
        return (LHArray*)this->getObjectAtIndex(index);
    }
    
    Point pointAtIndex(unsigned int index){
        return PointFromString(((__String*)this->getObjectAtIndex(index))->getCString());
    }

    std::string stringAtIndex(unsigned int index){
        return ((__String*)this->getObjectAtIndex(index))->getCString();
    }

};


#endif
