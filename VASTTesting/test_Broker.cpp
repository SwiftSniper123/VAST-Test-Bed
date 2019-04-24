/* THIS IS COMMENTED OUT BECAUSE OF THE OS VIRUS-PROTECTION'S 
TENDANCY TO BLOCK THIS .EXE FILE FROM RUNNING DUE TO THE TCP 
PORT CONNECTIONS OPENED IN THIS TEST.  EMF 4/19/2019 */

//#include "gtest/gtest.h"
//#include "socket.h"
//#include "Broker.h"
//#include <thread>
//
//using std::string;
//using tcpip::Socket;
//using tcpip::SocketException;
//using std::thread;
//
//void testCallback(Socket* s)
//{
//	s->accept();
//};
//
//
///* Default true test*/
//TEST(DefaultTest, testBrokerTrue)
//{
//	ASSERT_TRUE(true);
//}
//
///*SocketConnection - Tests that the socket listener cannot connect if no port is established, but connects to an existing port.
//Procedure:
//Pass:
//Fail:
//*/
//TEST(Test_Broker, SocketConnection)
//{
//	// if there is NO server, the listener throws a SocketException
//	Socket* testSocketListener;
//	try
//	{
//		testSocketListener = new Socket("localhost", 1337);
//		ASSERT_THROW(testSocketListener->connect(), SocketException);
//	}
//	catch (SocketException e)
//	{
//		ASSERT_TRUE(false);  // we didn't want this exception, so fail
//		string errorMsg = string(e.what());
//		delete testSocketListener;
//	}
//	catch (...)
//	{
//		ASSERT_TRUE(false); // we didn't want this exception, so fail
//		delete testSocketListener;
//	}
//	delete testSocketListener;
//	//----build a server------------------//
//	// now time to create a server for the listener to connect to 
//	Socket* testSocketServer;
//	try
//	{
//		//int testPortID = Socket::getFreeSocketPort();
//		testSocketListener = new Socket("localhost", 1337);
//		testSocketServer = new Socket(1337);
//		thread t1(testCallback, testSocketServer);
//		ASSERT_NO_THROW(testSocketListener->connect());
//		t1.join();
//	}
//	catch (...)
//	{
//		ASSERT_TRUE(false);  // we didn't want this exception, so fail
//	}
//
//	delete testSocketListener;
//	delete testSocketServer;
//}
//
//class TestSendBroker : public Broker
//{
//public:
//	TestSendBroker() : Broker("TestSend")
//	{};
//
//	void processData()
//	{
//		// does nothing
//	};
//};
//
///*BrokerCreation*/
//TEST(Test_Broker, BrokerCreation)
//{
//	// Create a Broker without a connection, check name is set.
//	TestSendBroker* tsb;
//	ASSERT_NO_THROW(tsb = new TestSendBroker());
//	ASSERT_EQ(tsb->getBrokerName(),"TestSend");
//
//	// Create a server to connect to, and finish setting the Port ID on Broker
//	Socket* testSocketServer;
//	try
//	{
//		//int testPortID = Socket::getFreeSocketPort();
//		testSocketServer = new Socket(1337);
//		thread t1(testCallback, testSocketServer);
//		ASSERT_NO_THROW(tsb->setPortID(1337));
//		//tsb->connect());
//		t1.join();
//	}
//	catch (...)
//	{
//		ASSERT_TRUE(false);  // we didn't want this exception, so fail
//	}
//	
//	// cleanup
//	delete tsb, testSocketServer;
//}
//
///*SocketSendandReceive - Tests that the Broker can send and receive
//*/
//TEST(Test_Broker, SocketSendandReceive)
//{
//
//}