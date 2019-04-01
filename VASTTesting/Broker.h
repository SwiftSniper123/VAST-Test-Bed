#pragma once
#include "socket.h"

using std::string;

class Broker 
{
private:
	/* */
	tcpip::Socket* _socket;
	
	/* Identification name for Exceptions and outputs.*/
	string _brokerName;

	int _portID;

public:
	/* Creates a Broker object that will be able to create a Socket once the Port 
	ID is chosen.
	name - the identification of the broker and where it is located in the system; i.e.
			"EnvironmentSender"*/
	Broker(string name) 
	{
		_brokerName = name;
		_portID = -1;
	};

	/* Returns the name that identifies this Broker and where it is located in the 
	system.*/
	string getBrokerName()
	{
		return _brokerName;
	};

	/* Creates the Socket, sets the port ID, and verifies that the port ID can be 
	connected to. Must be called after the server is established.*/
	void setPortID(int portID) 
	{
		_socket = new tcpip::Socket("localhost", portID);
		_portID = portID;
		_socket->connect();
	};

	/* Once the port ID has been declared in setPortID, this getter retrieves it.  
	If the Port has not been opened, this returns -1*/
	int getPortID() 
	{ 
		return _portID; 
	};

	/* Send or receive action, to be defined by the child of Broker.  Will call 
	the send or receive on the local socket.*/
	virtual void processData() = 0;
};