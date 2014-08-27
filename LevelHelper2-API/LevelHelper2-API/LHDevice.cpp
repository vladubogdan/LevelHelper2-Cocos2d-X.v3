//
//  LHDevice.cpp
//  Cocos2dX
//
//  Created by Bogdan Vladu on 19/05/14.
//
//

#include "LHDevice.h"
#include "LHDictionary.h"

#include "sstream"
std::string stringFromInt(const int& i){
    std::stringstream st;
    st << i;
    return st.str();
}


LHDevice::LHDevice(LHDictionary* dict){

    size    = dict->sizeForKey("size");
    suffix  = dict->stringForKey("suffix");
    ratio   = dict->floatForKey("ratio");
}

LHDevice* LHDevice::currentDeviceFromArray(const std::vector<LHDevice*>& devices){
    return LHDevice::deviceFromArrayWithSize(devices, LH_SCREEN_RESOLUTION);
}

LHDevice* LHDevice::deviceFromArrayWithSize(const std::vector<LHDevice*>& devices, Size size)
{
    for(size_t i = 0; i < devices.size(); ++i)
    {
        LHDevice* dev = devices[i];
        if(dev->getSize().equals(size))
        {
            return dev;
        }
    }
    
    if(devices.size()>0){
        return devices[0];
    }
    
    return NULL;
}

std::string LHDevice::devicePosition(LHDictionary* availablePositions, Size curScr){
    return availablePositions->stringForKey(stringFromInt((int)curScr.width)+"x"+stringFromInt((int)curScr.height));
}
