//
//  LHUtils.cpp
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 25/05/14.
//
//

#include "LHUtils.h"
#include "LHValue.h"

//we implement this function because its missing in Windows Phone 8 c++ lib.
float fmin(float x, float y){
    return x > y ? y : x;
}

//we implement this function because its missing in Windows Phone 8 c++ lib.
float fmax(float x, float y){
    return x > y ? x: y;
}

std::string LHUtils::getLastPathComponent(const std::string& path)
{
    
    size_t lastSlashPos = path.find_last_of("/");
    
    if(lastSlashPos != std::string::npos && lastSlashPos != path.length()-1) {
        return path.substr(lastSlashPos + 1, path.length() - lastSlashPos);
    }
    
    //maybe the path ends in a slash / so we should search backward after the last /
    
    if(lastSlashPos-1 > 0)
    {
        lastSlashPos = path.find_last_of("/", lastSlashPos-1);
        if(lastSlashPos != std::string::npos) {
            return path.substr(lastSlashPos + 1, path.length() - lastSlashPos);
        }
    }
    
    
    return path;
}

std::string LHUtils::removeLastPathComponent(const std::string& path)
{
    std::string lastPathComponent = LHUtils::getLastPathComponent(path);
    return path.substr(0, path.length()- lastPathComponent.length());
}

std::string LHUtils::getImagePathWithFilename(const std::string& filename,
                                           const std::string& folder,
                                           const std::string& suffix)
{
    return folder + suffix + "/" + filename;
}

unsigned long long LHUtils::LHMillisecondNow()
{
//    long lLastTime = 0;
    struct timeval stCurrentTime;
    
    gettimeofday(&stCurrentTime,NULL);
//    lLastTime = stCurrentTime.tv_sec*1000+stCurrentTime.tv_usec*0.001; //millseconds
//    return lLastTime;
    return (unsigned long long)stCurrentTime.tv_sec * 1000 + (unsigned long long)stCurrentTime.tv_usec*0.001;
    
}

LHValue* LHUtils::LHLinesIntersection(const Point& p1, const Point& p2, const Point& p3, const Point& p4)
{
    // Store the values for fast access and easy
    // equations-to-code conversion
    float x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
    float y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;
    
    float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    // If d is zero, there is no intersection
    if (d == 0) return nullptr;
    
    // Get the x and y
    float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
    float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
    float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;
    
    // Check if the x and y coordinates are within both lines
    if ( x < fmin(x1, x2) || x > fmax(x1, x2) ||
        x < fmin(x3, x4) || x > fmax(x3, x4) ) return NULL;
    if ( y < fmin(y1, y2) || y > fmax(y1, y2) ||
        y < fmin(y3, y4) || y > fmax(y3, y4) ) return NULL;
    
    // Return the point of intersection
    return LHValue::create(Point(x, y));;
}

Point LHUtils::LHPointNormalize(const Point& pt)
{
    Point temp;
    temp.x = pt.x/LHUtils::LHPointLength(pt);
    temp.y = pt.y/LHUtils::LHPointLength(pt);
    return temp;
}

Point LHUtils::LHPointScaled(const Point& pt, float val)
{
    return Point(pt.x*val, pt.y*val);
}

float LHUtils::LHPointLength(const Point& pt)
{
    return sqrtf(pt.x*pt.x + pt.y*pt.y);
}

double LHUtils::LHNormalAbsoluteAngleDegrees(double angle) {
    angle = fmod(angle, 360.0f);
    return angle >= 0 ? angle : (angle + 360.0f);
}

bool LHUtils::LHRectOverlapsRect(Rect r1,  Rect r2)
{
    return !(r1.origin.x + r1.size.width < r2.origin.x ||
             r1.origin.y + r1.size.height < r2.origin.y ||
             r1.origin.x > r2.origin.x + r2.size.width ||
             r1.origin.y > r2.origin.y + r2.size.height);
}

float LHUtils::LHRandomFloat(float Min, float Max){
//    return ((arc4random()%RAND_MAX)/(RAND_MAX*1.0))*(Max-Min)+Min;
    return ((rand()%RAND_MAX)/(RAND_MAX*1.0))*(Max-Min)+Min;
}


ValueVector LHUtils::Array_To_ValueVector(__Array* arr)
{
    ValueVector ret;
    
    Ref* obj;
    CCARRAY_FOREACH(arr, obj)
    {
        Value arrElement;
        
        __String* strVal = nullptr;
        __Dictionary* dictVal = nullptr;
        __Array* arrVal = nullptr;
        __Double* doubleVal = nullptr;
        __Bool* boolVal = nullptr;
        __Float* floatVal = nullptr;
        __Integer* intVal = nullptr;
        
        if ((strVal = dynamic_cast<__String *>(obj))) {
            arrElement = Value(strVal->getCString());
        } else if ((dictVal = dynamic_cast<__Dictionary*>(obj))) {
            arrElement = Dictionary_To_ValueMap(dictVal);
        } else if ((arrVal = dynamic_cast<__Array*>(obj))) {
            arrElement = Array_To_ValueVector(arrVal);
        } else if ((doubleVal = dynamic_cast<__Double*>(obj))) {
            arrElement = Value(doubleVal->getValue());
        } else if ((floatVal = dynamic_cast<__Float*>(obj))) {
            arrElement = Value(floatVal->getValue());
        } else if ((intVal = dynamic_cast<__Integer*>(obj))) {
            arrElement = Value(intVal->getValue());
        }  else if ((boolVal = dynamic_cast<__Bool*>(obj))) {
            arrElement = boolVal->getValue() ? Value(true) : Value(false);
        } else {
            CCASSERT(false, "the type isn't suppored.");
        }
        
        ret.push_back(arrElement);
    }
    return ret;
}

ValueMap LHUtils::Dictionary_To_ValueMap(__Dictionary* dict)
{
    ValueMap ret;
    DictElement* pElement = nullptr;
    CCDICT_FOREACH(dict, pElement)
    {
        Ref* obj = pElement->getObject();
        
        __String* strVal = nullptr;
        __Dictionary* dictVal = nullptr;
        __Array* arrVal = nullptr;
        __Double* doubleVal = nullptr;
        __Bool* boolVal = nullptr;
        __Float* floatVal = nullptr;
        __Integer* intVal = nullptr;
        
        Value dictElement;
        
        if ((strVal = dynamic_cast<__String *>(obj))) {
            dictElement = Value(strVal->getCString());
        } else if ((dictVal = dynamic_cast<__Dictionary*>(obj))) {
            dictElement = Dictionary_To_ValueMap(dictVal);
        } else if ((arrVal = dynamic_cast<__Array*>(obj))) {
            dictElement = Array_To_ValueVector(arrVal);
        } else if ((doubleVal = dynamic_cast<__Double*>(obj))) {
            dictElement = Value(doubleVal->getValue());
        } else if ((floatVal = dynamic_cast<__Float*>(obj))) {
            dictElement = Value(floatVal->getValue());
        } else if ((intVal = dynamic_cast<__Integer*>(obj))) {
            dictElement = Value(intVal->getValue());
        } else if ((boolVal = dynamic_cast<__Bool*>(obj))) {
            dictElement = boolVal->getValue() ? Value(true) : Value(false);
        } else {
            CCASSERT(false, "the type isn't suppored.");
        }
        
        const char* key = pElement->getStrKey();
        if (key && strlen(key) > 0)
        {
            ret[key] = dictElement;
        }
    }
    return ret;
}
