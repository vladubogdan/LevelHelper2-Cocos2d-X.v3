//This source file was generated automatically by LevelHelper 2
//based on the class template defined by the user.
//For more info please visit: http://www.gamedevhelper.com

#include "DemoCustomClass.h"
#include "LHScene.h"
#include "LHNodeProtocol.h"

DemoCustomClass* DemoCustomClass::customClassInstanceWithNode(Node* n)
{
	DemoCustomClass* instance = new DemoCustomClass();
	instance->setNode(n);
	return instance;
}

void DemoCustomClass::setPropertiesFromDictionary(LHDictionary* dict)
{

	if(dict->objectForKey("life"))
		setLife(dict->floatForKey("life"));

	if(dict->objectForKey("stringMember"))
		setStringMember(dict->stringForKey("stringMember"));

	if(dict->objectForKey("otherRobot"))
		otherRobot_uuid = dict->stringForKey("otherRobot");

	if(dict->objectForKey("alive"))
		setAlive(dict->boolForKey("alive"));

}

Node* DemoCustomClass::getOtherRobot(){
	if(!otherRobot && otherRobot_uuid.length()>0){
		LHScene* scene = (LHScene*)getNode()->getScene();
		otherRobot= scene->getChildNodeWithUUID(otherRobot_uuid);
	}
	return otherRobot;
}

