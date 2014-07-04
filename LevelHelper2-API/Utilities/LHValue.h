//
//  LHValue.h
//  Cocos2dX
//
//  Created by Bogdan Vladu on 19/05/14.
//
//
#ifndef __LevelHelper2API_VALUE__
#define __LevelHelper2API_VALUE__

#include "cocos2d.h"
using namespace cocos2d;

class LHValue : public Ref
{
public:

    explicit LHValue(const Point& v) { _pointValue = v;}
    explicit LHValue(const Color4F& v) { _color4FValue = v;}
    
    void setPoint(const Point& v){_pointValue = v;}
    Point getPoint() const {return _pointValue;}

    void setColor4F(const Color4F& v){_color4FValue = v;}
    Color4F getColor4F() const {return _color4FValue;}

    
    virtual ~LHValue(){}

    static LHValue* create(const Point& v)
    {
        LHValue* pRet = new LHValue(v);
        if (pRet)
        {
            pRet->autorelease();
        }
        return pRet;
    }
    
    static LHValue* create(const Color4F& v)
    {
        LHValue* pRet = new LHValue(v);
        if (pRet)
        {
            pRet->autorelease();
        }
        return pRet;
    }
    
private:
    Point _pointValue;
    Color4F _color4FValue;
};


#endif /* __LevelHelper2API_VALUE__ */
