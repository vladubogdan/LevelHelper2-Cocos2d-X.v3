//This file was generated automatically by LevelHelper 2
//based on the class template defined by the user.
//For more info please visit: http://www.gamedevhelper.com

#include "LHNodeProtocol.h"
#include "LHUserPropertyProtocol.h"

class DemoCustomClass : public LHUserPropertyProtocol
{
public:
	float getLife(){return life;}
	void setLife(const float& lifeValue){life = lifeValue;}

	std::string getStringMember(){return stringMember;}
	void setStringMember(const std::string& stringMemberValue){stringMember = stringMemberValue;}

	Node* getOtherRobot();
	void setOtherRobot(Node* otherRobotValue){otherRobot = otherRobotValue;}

	bool getAlive(){return alive;}
	void setAlive(const bool& aliveValue){alive = aliveValue;}

	static DemoCustomClass* customClassInstanceWithNode(Node* node);

	std::string getClassName(){return "DemoCustomClass";}

	virtual void setPropertiesFromDictionary(LHDictionary* dict);

private:
	float life;
	std::string stringMember;
	std::string otherRobot_uuid;
	Node* otherRobot;
	bool alive;
};
