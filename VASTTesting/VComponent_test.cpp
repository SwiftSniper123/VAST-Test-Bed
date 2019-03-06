//// Elise Feldt
//// Capstone 2018-2019
//// VComponent Unit Test
//
//#include <iostream>
//#include <vector>
////#include <cassert>
//#include "pch.h"
//#include "VComponent.h"
//
//using namespace std;
//
///* An extension of VComponent base class titled Ext_A for convenience.*/
//class Ext_A : public VComponent
//{
//public:
//
//	Ext_A(int &id) : VComponent() 
//	{
//		_numUpdate = 0;
//		_id = id;
//	};
//
//	void update()
//	{
//		*_id++;
//		cout << *_id << endl;
//	};
//
//	static const char type = 'A';
//
//private: 
//	int _numUpdate;
//	int* _id;
//}; // end of Ext_A
//
///* An extension of VComponent base class titled Ext_B for convenience.*/
//class Ext_B : public VComponent
//{
//public: 
//	Ext_B(int& id) : VComponent()
//	{
//		_numUpdate = 0;
//		_id = id;
//	};
//	
//	void update()
//	{
//		_id++;
//	};
//
//	static const char type = 'B';
//
//private:
//	int _numUpdate;
//	int _id;
//}; // end Ext_B
//
///* VCFactory is a helper factory for this test class to create many multiples 
//and call their inherited methods.*/
//class VCFactory
//{
//public: 
//	VCFactory()
//	{};
//	
//	VCFactory(int numToGen)
//	{
//		_num = numToGen;
//	};
//
//	void generateExt(char ext)
//	{
//		for (int i = 0; i < _num; i++)
//		{
//			switch (ext)
//			{
//			case 'A': _extA.push_back(new Ext_A(i));
//				break;
//			case 'B': _extB.push_back(new Ext_B(i));
//			}
//		}
//	}
//
//	void updateAll(char ext)
//	{
//		for (int i = 0; i < _num; i++)
//		{
//			switch (ext)
//			{
//			case 'A': _extA[i]->update();
//				break;
//			case 'B': _extB[i]->update();
//			}
//		}
//	};
//
//	void destroyAll()
//	{
//		for (unsigned int i = 0; i < _extA.size(); i++)
//		{
//			delete _extA[i];
//		}
//
//		for (unsigned int i = 0; i < _extB.size(); i++)
//		{
//			delete _extB[i];
//		}
//	};
//private:
//	int _num;
//	vector<VComponent*> _extA;
//	vector<VComponent*> _extB;
//}; // end of VCFactory class
//
//TEST(DefaultTest, TrueIsTrue) 
//{
//	EXPECT_TRUE(true);
//}
//
///* TestExtAisVComponent - Tests that Ext_A class is an extension of 
//	type VComponent.
//	Procedure
//		*Create an Ext_A object, then store it in a VComponent address.
//		Check the typeid of each address.  Create a different object,
//		check the typeid against the Ext_A object.
//	Pass
//		*The typeids checked should be equal for the addresses that 
//		share base types.  The typeids that do not share base types 
//		should not be equal typeids. 
//	Fail
//		*If a does not extend VComponent, or if typeid function 
//		assesses a as the same type as VCFactory.
//	*/
//TEST(VCTest, TestExtisVComponent)
//{
//	int i = 0;
//	Ext_A a(i);
//	VComponent& vcomp1 = a;	
//	ASSERT_TRUE(typeid(a) == typeid(vcomp1));
//
//	Ext_B b(i);
//	VComponent& vcomp2 = b;
//	ASSERT_TRUE(typeid(b) == typeid(vcomp2));
//
//	ASSERT_TRUE(typeid(a) != typeid(b));
//	ASSERT_TRUE(typeid(vcomp1) != typeid(vcomp2));
//}
//
//TEST(VCTest, ImplUpdate)
//{
//	int i = 0;
//	Ext_A a(i);
//	a.update();
//	EXPECT_EQ(i, 1);
//}
//
///*int main(int argc, char* argv[])
//{
//	testing::InitGoogleTest(&argc, argv);
//	return RUN_ALL_TESTS();
//}*/s