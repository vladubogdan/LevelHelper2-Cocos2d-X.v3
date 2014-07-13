//This header file was generated automatically by LevelHelper 2
//based on the class template defined by the user.
//For more info please visit: http://www.gamedevhelper.com

#ifndef __LEVELHELPER_API_CUSTOM_USER_PROPERTIES_HEADERS__
#define __LEVELHELPER_API_CUSTOM_USER_PROPERTIES_HEADERS__

#include "RobotUserProperty.h"
//If compiler gives an error with "... file not found" you need to add that file into your Xcode project or Make file.


#include "LHUserPropertyProtocol.h"

class LHUserCustomPropertiesManager
{
public:
	static LHUserPropertyProtocol* customClassInstanceWithName(const std::string& className, Node* node){
		if("RobotUserProperty" == className){ return RobotUserProperty::customClassInstanceWithNode(node);}
		return NULL;
	}

};


#endif //__LEVELHELPER_API_CUSTOM_USER_PROPERTIES_HEADERS__
