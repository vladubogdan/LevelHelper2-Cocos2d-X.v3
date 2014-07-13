//This file was generated automatically by LevelHelper 2
//based on the class template defined by the user.
//For more info please visit: http://www.gamedevhelper.com

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
