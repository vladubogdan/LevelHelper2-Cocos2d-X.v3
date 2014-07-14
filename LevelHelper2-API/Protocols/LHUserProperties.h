//This file was generated automatically by LevelHelper 2
//based on the class template defined by the user.
//For more info please visit: http://www.gamedevhelper.com

#ifndef __LEVELHELPER_API_USER_PROPERTIES_H__
#define __LEVELHELPER_API_USER_PROPERTIES_H__

#include "LHNodeProtocol.h"
#include "LHUserPropertyProtocol.h"


class RobotUserProperty : public LHUserPropertyProtocol
{
public:
	float getLife(){return life;}
	void setLife(const float& lifeValue){life = lifeValue;}

	Node* getConnection();
	void setConnection(Node* connectionValue){connection = connectionValue;}

	bool getActivated(){return activated;}
	void setActivated(const bool& activatedValue){activated = activatedValue;}

	std::string getModel(){return model;}
	void setModel(const std::string& modelValue){model = modelValue;}

	static RobotUserProperty* customClassInstanceWithNode(Node* node);

	std::string getClassName(){return "RobotUserProperty";}

	virtual void setPropertiesFromDictionary(LHDictionary* dict);

private:
	float life;
	std::string connection_uuid;
	Node* connection;
	bool activated;
	std::string model;
};


class SecondProperty : public LHUserPropertyProtocol
{
public:
	std::string getMemberA(){return memberA;}
	void setMemberA(const std::string& memberAValue){memberA = memberAValue;}

	float getMemberB(){return memberB;}
	void setMemberB(const float& memberBValue){memberB = memberBValue;}

	bool getMemberC(){return memberC;}
	void setMemberC(const bool& memberCValue){memberC = memberCValue;}

	static SecondProperty* customClassInstanceWithNode(Node* node);

	std::string getClassName(){return "SecondProperty";}

	virtual void setPropertiesFromDictionary(LHDictionary* dict);

private:
	std::string memberA;
	float memberB;
	bool memberC;
};



class LHUserCustomPropertiesManager
{
public:
	static LHUserPropertyProtocol* customClassInstanceWithName(const std::string& className, Node* node){
		if("RobotUserProperty" == className){ return RobotUserProperty::customClassInstanceWithNode(node);}
		if("SecondProperty" == className){ return SecondProperty::customClassInstanceWithNode(node);}
		return NULL;
	}

};

#endif //__LEVELHELPER_API_USER_PROPERTIES_H__
