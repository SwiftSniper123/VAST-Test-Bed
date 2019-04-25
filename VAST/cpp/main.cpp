//Virtual Autonomous System Testbed
//ODU Capstone 2018-2019

#include "..\h\VComponent.h"
#include <iostream>
#include <map>
#include<variant>
#include "..\h\VType.h"
#include "..\h\AV.h"
#include "..\h\Environment.h"
#include "..\h\TraciControlToSumo.h"
#include <Windows.h>


using namespace std;

class ExtendedVComponent
{
public:
	ExtendedVComponent(int id)
	{
		this_id = id;
	};

	void update()
	{
		cout << "\nExtendedVComponent" << this_id << " is updating." << endl;  
	};

private:
	int this_id;
};

int main(int argc, char **argv1)
{
	std::vector<unsigned char> charMessage;
	tcpip::Storage* testMessage = new tcpip::Storage();
	tcpip::Socket* s = new tcpip::Socket(1339);
	s->accept();

	try
	{
		s->receiveExact(*testMessage);
	}
	
	catch (tcpip::SocketException e)
	{
		e.what();
		delete testMessage;
		delete s;
	}

	//std::cout << testMessage->readUnsignedByte();
}
