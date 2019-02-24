#include "..\VAST\h\AV.h"
#include <iostream>
#include <vector>
#include "pch.h"
#include "..\VAST\h\VComponent.h"

using namespace std;

/* An extension of VComponent base class titled Ext_A for convenience.*/
class Ext_A : public VComponent
{
public:

	Ext_A(int* numUpdate) : VComponent()
	{
		_numUpdate = numUpdate;
	};

	void update()
	{
		*_numUpdate = *_numUpdate + 1;
	};

	static const char type = 'A';

private:
	int* _numUpdate;
}; // end of Ext_A

/* An extension of VComponent base class titled Ext_B for convenience.*/
class Ext_B : public VComponent
{
public:
	Ext_B(int* numUpdate) : VComponent()
	{
		_numUpdate = numUpdate;
	};

	void update()
	{
		*_numUpdate = *_numUpdate - 1;
	};

	static const char type = 'B';

private:
	int* _numUpdate;
}; // end Ext_B

/* VCFactory is a helper factory for this test class to create many multiples
and call their inherited methods.*/
class VCFactory
{
public:
	VCFactory(int* numToUpdate)
	{
		_numToUpdate = numToUpdate;
		totalCount = 0;
	};

	~VCFactory()
	{
		destroyAll();
	};

	void generateExt(int numToGen, char type)
	{
		totalCount += numToGen;
		for (int i = 0; i < numToGen; i++)
		{
			switch (type)
			{
			case 'A': extended.push_back(new Ext_A(_numToUpdate));
				break;
			case 'B': extended.push_back(new Ext_B(_numToUpdate));
				break;
			default :  ;// no default
			}
		}
	}

	void updateAll()
	{
		// Regardless of extended class's implemented type, all 
		// registered VComponents can be told to update()
		for (VComponent* vcomp : extended)
		{
			vcomp->update();
		}
	};

	int getTotalExtended()
	{
		return totalCount;
	}

	void destroyAll()
	{
		extended.clear();
	};
private:
	int* _numToUpdate;
	int totalCount;
	vector<VComponent*> extended;
}; // end of VCFactory class

TEST(DefaultTest, TrueIsTrue)
{
	EXPECT_TRUE(true);
}

/* TestExtAisVComponent - Tests that Ext_A class is an extension of
	type VComponent.
	Procedure
		*Create an Ext_A object, then store it in a VComponent address.
		Check the typeid of each address.  Create a different object,
		check the typeid against the Ext_A object.
	Pass
		*The typeids checked should be equal for the addresses that
		share base types.  The typeids that do not share base types
		should not be equal typeids.
	Fail
		*If a does not extend VComponent, or if typeid function
		assesses a as the same type as VCFactory.
	*/
TEST(VCTest, TestExtisVComponent)
{
	int i = 0;
	Ext_A a(&i);
	VComponent& vcomp1 = a;
	ASSERT_TRUE(typeid(a) == typeid(vcomp1));

	Ext_B b(&i);
	VComponent& vcomp2 = b;
	ASSERT_TRUE(typeid(b) == typeid(vcomp2));

	ASSERT_TRUE(typeid(a) != typeid(b));
	ASSERT_TRUE(typeid(vcomp1) != typeid(vcomp2));
}

/* TestImplUpdate - Tests that Ext_A and Ext_B extensions implement the 
		update() function differently but are called the same, via 
		VCFactory call to updateAll(), which is a serial call to update() 
	Procedure
		*Create an integer and pass its address through each extending
		object.  For as many objects as get created, the update() 
		function performs different implementations depending upon 
		whether called by Ext_A (increment 1) or Ext_B (decrement 1). 
	Pass
		*The final value of integer i is predicted correctly.
	Fail
		*The final value of integer i is incorrect.
	*/
TEST(VCTest, TestImplUpdate)
{
	// update just one
	int i = 0;
	Ext_A* a = new Ext_A(&i);
	a->update();
	EXPECT_EQ(i, 1);

	// using the factory, generate 100 of each
	VCFactory* factory = new VCFactory(&i);
	factory->generateExt(100, 'A');
	// 100 are definitely in the factory, 
	EXPECT_EQ(factory->getTotalExtended(), 100);
	// but until update() is called, integer i stays the same: 1
	EXPECT_EQ(i, 1);

	// Now all registered VComponents can be told to update()
	factory->updateAll();
	// and integer i should be: 101
	EXPECT_EQ(i, 101);

	// now generate 200 B type extensions, which are -1 updaters
	factory->generateExt(50, 'B');
	// 150 are definitely in the factory (100 A's, 50 B's), 
	EXPECT_EQ(factory->getTotalExtended(), 150);
	// but until update() is called, integer i stays the same: 101
	EXPECT_EQ(i, 101);

	// Now all registered VComponents can be told to update()
	// the previous 100 A's will still add 100, so the B's generated cause:
	// 101 + 100 - 50 = 151
	factory->updateAll();
	// and integer i should be: 151
	EXPECT_EQ(i, 151);

	//cleanup
	delete factory;
	delete a;
}

//TEST(TestCaseName, TestName) {
//  vector3 pos;
//  pos.x = 0;
//  pos.y = 0;
//  pos.z = 0;
//  float maxV = 10.0f;
//  AV *vehicle = new GroundVehicle("ground", maxV, 15.0f, pos, 1); //create AV object
//  EXPECT_EQ(maxV, vehicle->GetMaxV());
//  EXPECT_TRUE(true);
//}