//
//  LHUtils.h
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 25/05/14.
//
//

#ifndef __LevelHelper2API__LHUtils__
#define __LevelHelper2API__LHUtils__

#include <string>
#include "cocos2d.h"
using namespace cocos2d;

class LHPointValue;

class LHUtils
{
public:
    static std::string getLastPathComponent(const std::string& path);
    static std::string removeLastPathComponent(const std::string& path);
    
    static std::string getImagePathWithFilename(const std::string& filename,
                                                const std::string& folder,
                                                const std::string& suffix);

    static long LHMillisecondNow();

    //this methods are also available in cocos2d-x but it seams various versions have them in different places
    //so for simplicity and making the API work will all cocos2d-x v3 versions - I have my own methods over here.
    static LHPointValue* LHLinesIntersection(const Point& p1, const Point& p2, const Point& p3, const Point& p4);
    static Point LHPointNormalize(const Point& pt);
    static Point LHPointScaled(const Point& pt, float val);
    static float LHPointLength(const Point& pt);
};

#endif //__LevelHelper2API__LHUtils__
