//This file was generated automatically by LevelHelper 2
//based on the class template defined by the user.
//For more info please visit: http://www.gamedevhelper.com

#include "LHUtils.h"
#include "LHScene.h"
#include "LHUserProperties.h"

SecondProperty* SecondProperty::customClassInstanceWithNode(Node* n)
{
	SecondProperty* instance = new SecondProperty();
	instance->setNode(n);
	return instance;
}

void SecondProperty::setPropertiesFromDictionary(LHDictionary* dict)
{

	if(dict->objectForKey("memberA"))
		setMemberA(dict->stringForKey("memberA"));

	if(dict->objectForKey("memberB"))
		setMemberB(dict->floatForKey("memberB"));

	if(dict->objectForKey("memberC"))
		setMemberC(dict->floatForKey("memberC"));

}



RobotUserProperty* RobotUserProperty::customClassInstanceWithNode(Node* n)
{
	RobotUserProperty* instance = new RobotUserProperty();
	instance->setNode(n);
	return instance;
}

void RobotUserProperty::setPropertiesFromDictionary(LHDictionary* dict)
{

	if(dict->objectForKey("life"))
		setLife(dict->floatForKey("life"));

	if(dict->objectForKey("connection"))
		connection_uuid = dict->stringForKey("connection");

	if(dict->objectForKey("activated"))
		setActivated(dict->boolForKey("activated"));

	if(dict->objectForKey("model"))
		setModel(dict->stringForKey("model"));

}

Node* RobotUserProperty::getConnection(){
	if(!connection && connection_uuid.length()>0){
		LHScene* scene = (LHScene*)getNode()->getScene();
		connection= scene->getChildNodeWithUUID(connection_uuid);
	}
	return connection;
}


