#pragma once
#include "..\h\Environment.h"
#include "..\h\socket.h"

class SumoSim : public Environment
{
public:
	SumoSim();

protected:
	tcpip::Socket Listener;
};