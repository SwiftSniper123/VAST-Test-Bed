#pragma once
#include "socket.h"

using std::string;
class Broker 
{
private:
	/* */
	tcpip::Socket* _socket;
	
	/* Identification name for Exceptions and outputs.*/
	string brokerName;

public:
	/* Creates a Broker object that will be able to create a Socket once the Port 
	ID is chosen.
	name - the identification of the broker and where it is located in the system; i.e.
			"EnvironmentSender"*/
	Broker(string name) {};

	/* Returns the name that identifies this Broker and where it is located in the 
	system.*/
	string getBrokerName()
	{
		return "";
	};

	/* Creates the Socket, sets the port ID, and verifies that the port ID can be 
	connected to. */
	void setPortID(int portID) {};

	/* Once the port ID has been declared in setPortID, this getter retrieves it.  
	If the Port has not been opened, this returns -1*/
	int getPortID() { return 0; };

	/* Send or receive action, to be defined by the child of Broker.  Will call 
	the send or receive on the local socket.*/
	virtual void processData() = 0;
};