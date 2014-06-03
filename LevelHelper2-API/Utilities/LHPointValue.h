//
//  LHPointValue.h
//  Cocos2dX
//
//  Created by Bogdan Vladu on 19/05/14.
//
//
#ifndef __LevelHelper2API__POINT_VALUE__
#define __LevelHelper2API__POINT_VALUE__

#include "cocos2d.h"
using namespace cocos2d;

class LHPointValue : public Ref
{
public:
    LHPointValue(const Point& v) : _value(v) {}
    Point getValue() const {return _value;}

    virtual ~LHPointValue(){}
    
    static LHPointValue* create(const Point& v)
    {
        LHPointValue* pRet = new LHPointValue(v);
        if (pRet)
        {
            pRet->autorelease();
        }
        return pRet;
    }
    
private:
    Point _value;
};


#endif /* __LevelHelper2API__POINT_VALUE__ */
