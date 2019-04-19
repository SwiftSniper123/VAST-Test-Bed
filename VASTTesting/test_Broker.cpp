#include "gtest/gtest.h"
#include "socket.h"
#include "Broker.h"
#include <thread>
#include <vector>
#include <Windows.h>


using std::string;
using tcpip::Socket;
using tcpip::SocketException;
using std::thread;

void testCallback(Socket* s)
{
	s->accept();
};


/* Default true test*/
TEST(DefaultTest, testBrokerTrue)
{
	ASSERT_TRUE(true);
}

/*SocketConnection - Tests that the socket listener cannot connect if no port is established, but connects to an existing port.
Pass: A socket exception is thrown when testSocketListener trys to connect
Fail: Any other exception occurs, such as when the socket is created
*/
TEST(Test_Broker, SocketConnection)
{
	// if there is NO server, the listener throws a SocketException
	Socket* testSocketListener;
	try
	{
		testSocketListener = new Socket("localhost", 1337);
		ASSERT_THROW(testSocketListener->connect(), SocketException);
	}
	catch (SocketException e)
	{
		ASSERT_TRUE(false);  // we didn't want this exception, so fail
		string errorMsg = string(e.what());
		delete testSocketListener;
	}
	catch (...)
	{
		ASSERT_TRUE(false); // we didn't want this exception, so fail
		delete testSocketListener;
	}

	testSocketListener->close();
	delete testSocketListener;
	//----build a server------------------//
	// now time to create a server for the listener to connect to 
	Socket* testSocketServer;
	try
	{
		//int testPortID = Socket::getFreeSocketPort();
		testSocketListener = new Socket("localhost", 1337);
		testSocketServer = new Socket(1337);
		thread t1(testCallback, testSocketServer);
		ASSERT_NO_THROW(testSocketListener->connect());
		t1.join();
	}
	catch (...)
	{
		ASSERT_TRUE(false);  // we didn't want this exception, so fail
	}

	testSocketListener->close();
	testSocketServer->close();
	delete testSocketListener;
	delete testSocketServer;
}

class TestSendBroker : public Broker
{
public:
	TestSendBroker() : Broker("TestSend")
	{};

	void processData()
	{
		// does nothing
	};
};

/*BrokerCreation - Tests 
Pass: A socket exception is thrown when testSocketListener trys to connect
Fail: Any other exception occurs, such as when the socket is created
*/
TEST(Test_Broker, BrokerCreation)
{
	// Create a Broker without a connection, check name is set.
	TestSendBroker* tsb;
	ASSERT_NO_THROW(tsb = new TestSendBroker());
	ASSERT_EQ(tsb->getBrokerName(),"TestSend");

	// Create a server to connect to, and finish setting the Port ID on Broker
	Socket* testSocketServer;
	try
	{
		//int testPortID = Socket::getFreeSocketPort();
		testSocketServer = new Socket(1337);
		thread t1(testCallback, testSocketServer);
		ASSERT_NO_THROW(tsb->setPortID(1337));
		//tsb->connect());
		t1.join();
	}
	catch (...)
	{
		ASSERT_TRUE(false);  // we didn't want this exception, so fail
	}
	
	// cleanup
	testSocketServer->close();
	delete tsb, testSocketServer;
}

/*SocketSendandReceive - Creates a client and server socket, connects them, sends a message from the client to the server, and checks if the sent and received messages are the same.
Pass: The sent and received messages are the same
Fail: The sent and received messages aren't the same
*/
TEST(Test_Broker, SocketSendandReceive)
{
	tcpip::Socket* client = new Socket("localhost", 1338);
	tcpip::Socket* server = new Socket(1338);

	std::thread t1(testCallback, server);
	client->connect();
	t1.join();

	std::vector<unsigned char> testMessage = { 1, 2 };
	client->send(testMessage);
	std::vector<unsigned char> testMessageReceived = server->receive();

	ASSERT_TRUE(testMessageReceived == testMessage);

	client->close();
	server->close();

	delete client;
	delete server;
}

/* SocketSendandReceiveString - Creates a client and server socket, connects them, sends a string message from the client to the server, and checks if the sent and received messages are the same.
Pass: The received message is equal to "Test", the sent message.
Fail: The received message does not equal "Test"
*/
TEST(Test_Broker, SocketSendandReceiveString)
{
	tcpip::Socket* client = new Socket("localhost", 1338);
	tcpip::Socket* server = new Socket(1338);

	std::thread t1(testCallback, server);
	client->connect();
	t1.join();

	tcpip::Storage* testMessage = new tcpip::Storage();
	testMessage->writeString("Test");
	client->sendExact(*testMessage);

	tcpip::Storage* receiveMessage = new tcpip::Storage();
	server->receiveExact(*receiveMessage);


	ASSERT_TRUE(receiveMessage->readString() == "Test");

	client->close();
	server->close();

	delete client;
	delete server;
}