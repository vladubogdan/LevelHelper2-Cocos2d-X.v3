//
//  LHUtils.cpp
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 25/05/14.
//
//

#include "LHUtils.h"
#include "LHPointValue.h"

std::string LHUtils::getLastPathComponent(const std::string& path)
{
    
    size_t lastSlashPos = path.find_last_of("/");
    
//    printf("PATH IS %s length %d slashPos %d\n", path.c_str(), path.length(), lastSlashPos);

    
    if(lastSlashPos != std::string::npos && lastSlashPos != path.length()-1) {
        return path.substr(lastSlashPos + 1, path.length() - lastSlashPos);
    }
    
    //maybe the path ends in a slash / so we should search backward after the last /
    
    if(lastSlashPos-1 > 0)
    {
        lastSlashPos = path.find_last_of("/", lastSlashPos-1);
//        printf("NOW slashPos %d\n", lastSlashPos);
        
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

LHPointValue* LHUtils::LHLinesIntersection(const Point& p1, const Point& p2, const Point& p3, const Point& p4)
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
    return LHPointValue::create(Point(x, y));;
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