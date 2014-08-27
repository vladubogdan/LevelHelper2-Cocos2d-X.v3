//
//  LHDevice.h
//  Cocos2dX
//
//  Created by Bogdan Vladu on 19/05/14.
//
//

#ifndef __LevelHelper2API__LHDevice__
#define __LevelHelper2API__LHDevice__

#include "cocos2d.h"

#define LH_SCREEN_RESOLUTION ( Director::getInstance()->getOpenGLView()->getFrameSize() )


class LHDictionary;
using namespace cocos2d;

class LHDevice
{
public:
    Size        size;
    float       ratio;
    std::string suffix;

    LHDevice(LHDictionary* dict);
    
    virtual ~LHDevice(){}
    Size getSize(){return size;}
    std::string getSuffix(){return suffix;}
    float getRatio(){return ratio;}
    
    static LHDevice* currentDeviceFromArray(const std::vector<LHDevice*>& devices);
    static LHDevice* deviceFromArrayWithSize(const std::vector<LHDevice*>& devices, Size size);
    static std::string devicePosition(LHDictionary* availablePositions, Size curScr);
};


#endif /* defined(__LevelHelper2API__LHDevice__) */
