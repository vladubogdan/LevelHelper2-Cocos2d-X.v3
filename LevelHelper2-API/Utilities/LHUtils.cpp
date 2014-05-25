//
//  LHUtils.cpp
//  LevelHelper2-Cocos2d-X.v3
//
//  Created by Bogdan Vladu on 25/05/14.
//
//

#include "LHUtils.h"


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
