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

class LHUtils
{
public:
    static std::string getLastPathComponent(const std::string& path);
    static std::string removeLastPathComponent(const std::string& path);
    
    static std::string getImagePathWithFilename(const std::string& filename,
                                                const std::string& folder,
                                                const std::string& suffix);

    
};

#endif //__LevelHelper2API__LHUtils__
