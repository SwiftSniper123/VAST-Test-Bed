#include <vector>
#include "gtest/gtest.h"
#include "VType.h"
#include <exception>
#include "VComponent.h"

using std::cout;
using std::vector;
using std::exception;

TEST(Test_VType, DefaultTrueIsTrue)
{
	EXPECT_TRUE(true);
}

/* Emergency cleanup - helper function for SPecialArrayFunctions
Provides a means to fail a test and also cleanup possible memory leaks.*/
bool emergencyCleanup(VType* trash)
{
	delete trash;
	return false;
}

/* IsVType - Tests that each extended VType can be stored or moved as a VType, and reflects its type in the type field. 
Procedure: Create one of each extended VType and store them in the vTypes vector, checking their type field. 
Pass: All objects can be stored and types can be verified. 
Fail: An object cannot be stored or a type is not correct. 
*/
TEST(Test_VType, IsVType)
{
	// Object 0 - String
	String* s = new String;
	vector<VType*> vTypes;
	vTypes.push_back(s);
	ASSERT_TRUE(vTypes.at(0)->isA(STRING_TYPE));

	// Object 1 - Double
	Double* d = new Double;
	vTypes.push_back(d);
	ASSERT_TRUE(vTypes.at(1)->isA(DOUBLE_TYPE));

	// Object 2 - Integer
	Integer* i = new Integer;
	vTypes.push_back(i);
	ASSERT_TRUE(vTypes.at(2)->isA(INTEGER_TYPE));

	// Object 3 - Boolean
	Boolean* b = new Boolean;
	vTypes.push_back(b);
	ASSERT_TRUE(vTypes.at(3)->isA(BOOLEAN_TYPE));

	// Object 4 - Vector3
	Vector3* v = new Vector3;
	vTypes.push_back(v);
	ASSERT_TRUE(vTypes.at(4)->isA(VECTOR3_TYPE));

	// Object 5 - Array
	Array* a = new Array;
	vTypes.push_back(a);
	ASSERT_TRUE(vTypes.at(5)->isA(ARRAY_TYPE));
	
	vTypes.clear();
}

/* GetSQLite3Type - Tests that each extended VType can provide its SQLite3 counterpart
Procedure: Create one of each extended VType and request the SQLite3 text.
Pass: All types were provided as predicted.  
Fail: An internal type has changed.  
*/
TEST(Test_VType, GetSQLite3Type)
{
	String* s = new String();
	Double* d = new Double();
	Integer* i = new Integer();
	Boolean* b = new Boolean();

	ASSERT_EQ(s->getSQLite3Text(), "VARCHAR(255)");
	ASSERT_EQ(d->getSQLite3Text(), "DOUBLE");
	ASSERT_EQ(i->getSQLite3Text(), "INT");
	ASSERT_EQ(b->getSQLite3Text(), "BOOLEAN");

	delete s, d, i, b;
}

/* GetValue - Tests that each extended VType'svalue can be retrieved through the value() function.
Procedure: Create one of each extended VType and confirm the value passes through the constructor and can be retrieved again by .value()
Pass: All objects return their passed value.
Fail: The constructor does not set the value properly in the object.
*/
TEST(Test_VType, GetValueFromPassingToConstructor)
{
	String* s_obj = new String("1");
	EXPECT_EQ(s_obj->value(), "1");

	Double* d_obj = new Double(2.0);
	EXPECT_EQ(d_obj->value(), 2.0);

	Integer* i_obj = new Integer(3);
	EXPECT_EQ(i_obj->value(), 3);

	Boolean* b_obj = new Boolean(true);
	EXPECT_EQ(b_obj->value(), true);

	Vector3* v_obj = new Vector3(5.0,5.0,5.0);
	EXPECT_EQ(v_obj->x(), 5.0);
	EXPECT_EQ(v_obj->y(), 5.0);
	EXPECT_EQ(v_obj->z(), 5.0);

	Array* a_obj = new Array("1,2,3,4,5,6");
	// .value() returns a string representation of the Array objects
	EXPECT_TRUE(a_obj->value().compare("1,2,3,4,5,6") == 0);

	//cleanup
	delete s_obj, b_obj, d_obj, i_obj, v_obj;
	//EXPECT_DEATH(delete a_obj, "Wrong message.");

}

/* SetValue - Tests that each extended VType's value can be changed. Also tests that the default constructor generates the default value.
Procedure: Create one of each extended VType with default constructor, change the value with value(...) and check to see that it changed as anticipated.
Pass: All objects' values can be modified with value(...) call.
Fail: An object value cannot be modified correctly.
*/
TEST(Test_VType, SetValueFromDefaultConstructor)
{
	// String default is "" empty string
	String* s = new String;
	EXPECT_TRUE(s->value().compare("") == 0);
	// now change the value
	string test1 = "test1";
	s->value(test1);
	EXPECT_FALSE(s->value().compare("") == 0);
	EXPECT_TRUE(s->value().compare(test1) == 0);
	EXPECT_TRUE(s->value().compare("test1") == 0);	

	// Double default is 0.0 
	Double* d = new Double;
	EXPECT_TRUE(d->value() == 0.0);
	// now change the value
	double test2 = 2.0;
	d->value(test2);
	EXPECT_FALSE(d->value() == 0.0);
	EXPECT_TRUE(d->value() == test2);
	EXPECT_TRUE(d->value() == 2.0);	

	// Integer default is 0
	Integer* i = new Integer;
	EXPECT_TRUE(i->value() == 0);
	// now change the value
	int test3 = 3;
	i->value(test3);
	EXPECT_FALSE(i->value() == 0);
	EXPECT_TRUE(i->value() == test3);
	EXPECT_TRUE(i->value() == 3);
	
	// Boolean default is false
	Boolean* b = new Boolean;
	EXPECT_TRUE(b->value() == false);
	// now change the value
	bool test4 = true;
	b->value(test4);
	EXPECT_FALSE(b->value() == false);
	EXPECT_TRUE(b->value() == test4);
	EXPECT_TRUE(b->value() == true);

	// Vector3 default is {0,0,0} where each respective place is x,y,z
	Vector3* v = new Vector3;
	EXPECT_TRUE(v->x() == 0.0);
	EXPECT_TRUE(v->y() == 0.0);
	EXPECT_TRUE(v->z() == 0.0);
	// now change the value
	double test5_x = 5.0;
	double test5_y = 5.0;
	double test5_z = 5.0;
	v->value(test5_x, test5_y, test5_z);
	EXPECT_FALSE(v->x() == 0.0);
	EXPECT_FALSE(v->y() == 0.0);
	EXPECT_FALSE(v->z() == 0.0);
	EXPECT_TRUE(v->x() == test5_x);
	EXPECT_TRUE(v->y() == test5_y);
	EXPECT_TRUE(v->z() == test5_z);
	EXPECT_TRUE(v->x() == 5.0);
	EXPECT_TRUE(v->y() == 5.0);
	EXPECT_TRUE(v->z() == 5.0);
	stringstream ss;
	ss << test5_x << "," << test5_y << "," << test5_z;
	EXPECT_TRUE(v->value().compare(ss.str()) == 0);

	// Array default is size 1, String with empty string "" value
	//Array* testDestructorFirst = new Array;
	// this next EXPECT_DEATH should Fail: until the destructor is properly implemented.
	//EXPECT_DEATH(delete testDestructorFirst, "Wrong message.");
	Array* a = new Array;
	EXPECT_TRUE(a->value().compare("") == 0);
	// now change the value in the array
	string test6 = "1,2,3,4,5,6";
	a->value(test6);
	EXPECT_FALSE(a->value().compare("") == 0);
	EXPECT_TRUE(a->value().compare(test6) == 0);
	EXPECT_TRUE(a->value().compare("1,2,3,4,5,6") == 0);	

	// cleanup
	delete s, d, i, v, a;
	//EXPECT_DEATH(delete a, "Wrong message.");

}

/* SpecialStringFunctions - Tests that each special String function operates as predicted.  These include a length() accessor and a concat(...) mutator.
Procedure: For length()	create two strings and set a String object by one of them.  Check the length.  Set the String object by the other string value.  Check the length.  Set the String object to an empty string.  Check the length. For concat(...)	using the original String, call concat and then verify that the return is not a nullptr, and that the new value of String is the combined string values.
Pass: Functions length and concat are implemented correctly.
Fail: Functions length and concat are not implemented correctly, and/or a new String is not returned from concat.
*/
TEST(Test_VType, SpecialStringFunctions)
{
	// Testing length //
	string length1 = "1";
	string length5 = "12345";
	String* s = new String(length1);
	EXPECT_EQ(s->length(), length1.length());
	// now change the value and test length again
	s->value(length5);
	EXPECT_EQ(s->length(), length5.length());
	// now set the value to "" and test the length
	s->value("");
	EXPECT_EQ(s->length(),0);

	// Testing Concatenation with string
	// the value of s is reset to "12345"
	s->value("12345");
	EXPECT_EQ(s->value().compare(length5), 0);
	// reuse length1 to concatenate s - twice with "1", so "11"
	EXPECT_TRUE((s->concat(length1)) != nullptr);
	EXPECT_EQ((s->concat(length1))->value(), string("1234511"));

	// Testing Concatenation with integer
	// the value of s is currently "1234511"
	EXPECT_TRUE((s->concat(6)) != nullptr);
	EXPECT_EQ((s->concat(6))->value(), string("123451166"));
	
	// Testing Concatenation with double
	// the value of s is currently "123451166"
	EXPECT_TRUE((s->concat(7.777)) != nullptr);
	EXPECT_EQ((s->concat(8.8))->value(), string("1234511667.7778.8"));

	// Testing == operator overload
	// String exactly equals char* and vice versa
	EXPECT_TRUE(*s == "1234511667.7778.8");
	EXPECT_TRUE("1234511667.7778.8" == *s);
	// String exactly equals string and vice versa
	EXPECT_FALSE(*s == length5);
	EXPECT_FALSE(length5 == *s);
	String* rhs = new String("1234511667.7778.8");
	// String exactly equals String
	EXPECT_TRUE(*s == *rhs);

	// Testing != operator overload - exact opposite
	EXPECT_FALSE(*rhs != "1234511667.7778.8");
	EXPECT_FALSE("1234511667.7778.8" != *s);
	// String exactly equals string and vice versa
	EXPECT_TRUE(*s != length5);
	EXPECT_TRUE(length5 != *s);
	// String exactly equals String
	EXPECT_FALSE(*s != *rhs);
	
	// cleanup of pointers
	delete s, rhs;
}

/* SpecialDoubleFunctions - Tests that each special Double function operates like a double would.
Procedure: For operator+ overload create a Double and two doubles.  Interchange how they are combined between left hand side and right hand side addition with +.  For operator- overload		create a Double and two doubles.  Interchange how they are combined between left hand side and right hand side subtraction with -. 
Pass: Resulting doubles are the correct values, and the original Double values are untouched.  A Double can be set with these results.
Fail: Arithmetic was incorrect or functions are not implemented.
*/
TEST(Test_VType, SpecialDoubleFunctions)
{
	// Testing addition with + to return double
	Double* d1 = new Double; // 0.0 default
	double one = 1.0;
	double two = 2.0;
	EXPECT_TRUE(d1->value() == 0.0); // checked 0.0
	// result of 0.0(Double) + 1.0(double) is 1.0 double
	EXPECT_TRUE((*d1 + one) == 1.0);
	// result of 2.0(double) + 0.0(Double) is 2.0 double
	EXPECT_TRUE((two + *d1) == 2.0);
	Double* d3 = new Double(3.0);
	// result of 0.0(Double) + 3.0(Double) is 3.0 double
	EXPECT_TRUE((*d1 + *d3) == 3.0);
	// checking that original values are unchanged
	EXPECT_TRUE(d1->value() == 0.0);
	EXPECT_TRUE(d3->value() == 3.0);
	d3->value(two + *d1);
	EXPECT_TRUE(d3->value() == 2.0);
	
	// Testing subtraction with - to return double
	Double* d2 = new Double(2.0);
	double three = 3.0;
	double four = 4.0;
	EXPECT_TRUE(d2->value() == 2.0); // checked 0.0
	// result of 2.0(Double) - 3.0(double) is -1.0 double
	EXPECT_TRUE((*d2 - three) == -1.0);
	// result of 4.0(double) - 2.0(Double) is 2.0 double
	EXPECT_TRUE((four - *d2) == 2.0);
	Double* d4 = new Double(4.0);
	// result of 2.0(Double) + 4.0(Double) is 6.0 double
	EXPECT_TRUE((*d2 + *d4) == 6.0);
	// checking that original values are unchanged
	EXPECT_TRUE(d2->value() == 2.0);
	EXPECT_TRUE(d4->value() == 4.0);
	d4->value(four + *d2);
	EXPECT_EQ(d4->value(), 6.0);
	
	// Testing formatting the double
	// this function is not currently implemented

	// cleanup
	delete d1, d2, d3, d4;
}

/* SpecialIntegerFunctions - Tests that each special Integer function operates like an integer would.
Procedure For operator+ overload create an Integer and two integers. Interchange how they are combined between left hand side and right hand side addition with +. For operator- overload	create an Integer and two integers. Interchange how they are combined between left hand side and right hand side subtraction with -. For asDouble() create an integer, verify its value, then set a new Double by calling the asDouble() function.  
Pass: For Arithmetic overloads, resulting integers are the correct values, and the original Integer values are untouched.  An Integer can be set with these results. For asDouble(), the original integer is still integer, new Double matches double cast value of original integer
Fail: Arithmetic was incorrect or functions are not implemented.
*/
TEST(Test_VType, SpecialIntegerFunctions)
{
	// Testing addition with + to return int
	Integer* i1 = new Integer; // 0 default
	int one = 1;
	int two = 2;
	EXPECT_TRUE(i1->value() == 0); // checked 0
	// result of 0(Integer) + 1(int) is 1 int
	EXPECT_TRUE((*i1 + one) == 1);
	// result of 2(int) + 0(Integer) is 2 int
	EXPECT_TRUE((two + *i1) == 2);
	Integer* i3 = new Integer(3);
	// result of 0(Integer) + 3(Integer) is 3 int
	EXPECT_TRUE((*i1 + *i3) == 3);
	// checking that original values are unchanged
	EXPECT_TRUE(i1->value() == 0);
	EXPECT_TRUE(i3->value() == 3);
	i3->value(two + *i1);
	EXPECT_TRUE(i3->value() == 2);

	// Testing subtraction with - to return int
	Integer* i2 = new Integer(2);
	int three = 3;
	int four = 4;
	EXPECT_TRUE(i2->value() == 2); // checked 0
	// result of 2(Integer) - 3(int) is -1 int
	EXPECT_TRUE((*i2 - three) == -1);
	// result of 4(int) - 2(Integer) is 2 int
	EXPECT_TRUE((four - *i2) == 2);
	Integer* i4 = new Integer(4);
	// result of 2(Integer) + 4(Integer) is 6 int
	EXPECT_TRUE((*i2 + *i4) == 6);
	// checking that original values are unchanged
	EXPECT_TRUE(i2->value() == 2);
	EXPECT_TRUE(i4->value() == 4);
	i4->value(four + *i2);
	EXPECT_EQ(i4->value(), 6);
	
	// Testing conversion to Double
	Integer* i = new Integer(1);
	EXPECT_TRUE(i->value() == 1);
	Double* d = i->asDouble();
	EXPECT_TRUE(i->value() == 1);
	EXPECT_TRUE(d->value() == (double)1);

	delete i, d, i1, i2, i3, i4;
}

/* SpecialBooleanFunctions - Tests that each special Boolean function operates like an bool would.
Procedure: For operator|| overload create an Boolean and two bools. Interchange how they are combined between left hand side and right hand side logical assessment with ||. For operator&& overload		create an Boolean and two bools. Interchange how they are combined between left hand side and right hand side logical assessment with &&.
Pass: Logical operator overloads:	Resulting bools are the correct values. A Boolean can be set with these results.
Fail: Logical assessment was incorrect or functions are not implemented.
*/
TEST(Test_VType, SpecialBooleanFunctions)
{
	// Testing || comparison  - mixing Boolean and bool types to get bool result
	Boolean* b1 = new Boolean; // false default
	bool truthy = true;
	bool falsy = false;
	EXPECT_TRUE(b1->value() == false); // double-checked false
	// result of false(Boolean) OR truth(bool) is true bool
	EXPECT_TRUE(*b1 || truthy == true);
	Boolean* b2 = new Boolean;
	// result of false(Boolean) OR false(bool) is false bool
	EXPECT_TRUE(*b1 || falsy == false);
	// result of true(bool) OR false(Boolean) is true(bool)
	EXPECT_TRUE((truthy || b2) == true);
	// result of false(Boolean) OR false(Boolean) is false bool
	EXPECT_EQ(b1->value(), false);
	EXPECT_EQ(b2->value(), false);
	EXPECT_EQ((*b1 || *b2), false);


	// Testing && comparison - mixing Boolean and bool types to get bool result
	EXPECT_TRUE(b1->value() == false); // still false
	// result of false(Boolean) AND truth(bool) is false(bool)
	EXPECT_EQ(*b1 && truthy, false);
	// result of false(Boolean) AND false(Boolean) is false
	EXPECT_EQ((*b1 && *b2), false);
	// next set b1 and b2 to true 
	b1->value(true);
	b2->value(true);
	// result of true(bool) AND true(Boolean) is true (bool)
	EXPECT_TRUE((truthy && *b2) == true);
	// test true(Boolean) AND true(bool) is true(bool)
	EXPECT_TRUE((*b1 && truthy) == true);
	// test true(Boolean) AND true(Boolean) is true(bool)
	EXPECT_TRUE((*b1 && *b2) == true);
	

	// cleanup
	delete b1, b2;
}

/* SpecialArrayFunctions - Tests that the Array special functions operate as predicted.
Procedure: For the destructor function, create an empty Array, add a VType, then destroy the array and verify that the VType object was also destroyed by checking that it was set to nullptr.  
For getStowType, create different arrays with different internal VTypes and request their type.
For add and arraySize functions, add VTypes and request the size from each Array.  
For the index return functions, try successful and unsuccessful (exception-throwing) examples on each Array.
Pass: All special Array functions return values or exceptions as predicted.
Fail: Memory allocation errors because of element misplacement, or exceptions were not thrown.  
*/
TEST(Test_VType, SpecialArrayFunctions)
{
	// Tests Array destructor
	Array* a = new Array();
	Integer* i = new Integer(1);
	a->add(i);
	EXPECT_NO_FATAL_FAILURE(delete a);
	
	// Tests getStowType by testing each Array constructor
	Array* a1 = new Array("one");			// 1 default String
	Array* a2 = new Array(2);				// 2 default Strings
	Array* a3 = new Array(3, INTEGER_TYPE); // 3 default Integers
	VType** d_array = new VType*[4];
	d_array[0] = new Double();
	d_array[1] = new Double();
	d_array[2] = new Double();
	d_array[3] = new Double();
	Array* a4 = new Array(4, d_array);		
	vector<VType*> b_vector;
	b_vector.push_back(new Boolean(true)); // 0 index
	b_vector.push_back(new Boolean(false));// 1 
	b_vector.push_back(new Boolean(true)); // 2
	b_vector.push_back(new Boolean(false));// 3
	b_vector.push_back(new Boolean(true)); // 4
	Array* a5 = new Array(b_vector);
	EXPECT_EQ(a1->getStowType(),STRING_TYPE);
	EXPECT_EQ(a2->getStowType(),STRING_TYPE);
	EXPECT_EQ(a3->getStowType(),INTEGER_TYPE);
	EXPECT_EQ(a4->getStowType(),DOUBLE_TYPE);
	EXPECT_EQ(a5->getStowType(),BOOLEAN_TYPE);

	//// Tests setStowType - this is not implemented functionality 
	//EXPECT_TRUE(a1->setStowType(VECTOR3_TYPE)); // won't work
	//EXPECT_FALSE(a1->getStowType().compare(VECTOR3_TYPE) == 0);
	//EXPECT_TRUE(a1->getStowType().compare(STRING_TYPE) == 0);
	//// try to convert the whole array to a type that doesn't mesh
	//EXPECT_FALSE(a1->setStowType(BOOLEAN_TYPE));
	//// add the unlike type anyway (at slot 2)
	//Boolean* addB = new Boolean(true);
	//a1->add(addB);
	//// the type should have automatically set to VType again
	//EXPECT_TRUE(a1->getStowType().compare(V_TYPE) == 0);
	//// setting the type to String should always work
	//EXPECT_TRUE(a1->setStowType(STRING_TYPE));

	// Tests that VType objects can be added to the Array 
	EXPECT_NO_FATAL_FAILURE(a1->add(new Boolean(true)));
	EXPECT_EQ(a1->getStowType(), V_TYPE); // changes to VType to be more generic
	EXPECT_NO_FATAL_FAILURE(a2->add(new Boolean(true)));
	EXPECT_NO_FATAL_FAILURE(a3->add(new Integer(1)));
	EXPECT_NO_FATAL_FAILURE(a4->add(new Double(2.0)));
	EXPECT_NO_FATAL_FAILURE(a5->add(new Boolean(true)));

	// Tests array size function
	EXPECT_TRUE(a1->arraySize() == 2);
	EXPECT_TRUE(a2->arraySize() == 3);
	EXPECT_TRUE(a3->arraySize() == 4);
	EXPECT_TRUE(a4->arraySize() == 5);
	EXPECT_TRUE(a5->arraySize() == 6);

	// try out Arrays of different objects and getStowType
	String* s4 = new String();
	Double* d4 = new Double();
	Integer* i4 = new Integer();
	delete d_array[1];
	delete d_array[2];
	delete d_array[3];
	d_array[1] = s4;
	d_array[2] = d4;
	d_array[3] = i4;
	delete a4; // clean out the Array at a4
	a4 = new Array(4, d_array);
	EXPECT_EQ(a4->arraySize(), 4);
	EXPECT_TRUE(a4->isA(ARRAY_TYPE));
	EXPECT_TRUE(s4->isA(STRING_TYPE));
	EXPECT_TRUE(d4->isA(DOUBLE_TYPE));
	EXPECT_TRUE(i4->isA(INTEGER_TYPE));
	EXPECT_EQ(a4->getStowType(),V_TYPE);
	delete[] d_array; // no longer needed

	// Tests index operator retrieves by type, or 
	EXPECT_EQ(a1->at_String(0)->value(),"one"); // a1 has 2
	EXPECT_EQ(a1->at_Boolean(1)->value(),true);
	EXPECT_EQ(a1->at_Boolean(2), nullptr);
	EXPECT_EQ(a2->at_String(0)->value(),""); // a2 has 3
	EXPECT_EQ(a2->at_String(1)->value(),"");
	EXPECT_EQ(a2->at_Boolean(2)->value(),true);
	EXPECT_EQ(a2->at_Vector3(3), nullptr);
	EXPECT_EQ(a3->at_Integer(0)->value(),0); // a3 has 4
	EXPECT_EQ(a3->at_Integer(1)->value(),0);
	EXPECT_EQ(a3->at_Integer(2)->value(),0);
	EXPECT_EQ(a3->at_Integer(2)->value(),0);
	EXPECT_EQ(a5->at_Boolean(4)->value(),true);
	// also test that the original object still exists
	EXPECT_TRUE(a3->at_Integer(0) != nullptr);
	EXPECT_TRUE(a3->at_Integer(0)->value() == 0);
	String* test = new String(a4->at_VType(1));
	EXPECT_TRUE(*test == "");
	// test exceptions for invalid requests
	EXPECT_ANY_THROW(a1->at_String(-1));  // no -1 index in Array
	EXPECT_ANY_THROW(a1->at_Double(1));   // no Double in a Boolean spot
	   
	// cleanup
	b_vector.clear(); // no longer needed
	delete a1, a2, a3, a4, a5, test;
	//EXPECT_TRUE(addB == nullptr ? true : emergencyCleanup(addB));
}

typedef map<string, VType*> dataMap;

/* MapOfVTypes - Tests that the VTypes can be placed in a std map and retrieved by name.
Procedure: Create an update map, add VTypes with a string key, and then iterate through the map and reassign those VTypes to other variables based on the internal type attribute.
Pass: All VTypes can added and can be removed and recreated by VType passed into their constructor.  The string values of the VTypes in the map match with the new string value in the specific type object.
Fail: A type that has been added does not contain the proper type attribute or string value.
*/
TEST(Test_VType, MapOfVTypes)
{
	// Tests creating VType extensions, passing them into map, 
	// and then retrieving them again by name
	dataMap updateMap;
	updateMap.emplace(string("attribute1"), new String("init1"));
	updateMap.emplace(string("attribute2"), new Double(2.0));

	// now get values out of map
	String* s;
	Double* d;
	for (auto mapIterator = updateMap.begin(); mapIterator != updateMap.end(); ++mapIterator)
	{
		if (mapIterator->second->isA(STRING_TYPE))
			s = new String(mapIterator->second);
		else if (mapIterator->second->isA(DOUBLE_TYPE))
			d = new Double(mapIterator->second);
	}
	ASSERT_EQ(updateMap.at("attribute1")->s_value(), s->s_value());
	ASSERT_EQ(updateMap.at("attribute2")->s_value(), d->s_value());
	updateMap.clear();
	delete s, d;
}

/* An extension of VComponent base class titled Ext_A for convenience.*/
class Avatar : public VComponent
{
public:

	Avatar(string _name, double init_x, double init_y, double init_z, bool b) : VComponent()
	{
		_originalDataMap = new map<string, VType*>;
		// initial map values
		_originalDataMap->emplace("name", new String(_name));
		_originalDataMap->emplace("x", new Double(init_x));
		_originalDataMap->emplace("y", new Double(init_y));
		_originalDataMap->emplace("z", new Double(init_z));
		_originalDataMap->emplace("fact", new Boolean(b));
	};

	~Avatar()
	{
		_originalDataMap->clear();
	}

	void update(timestamp t, dataMap _updateMap)
	{
		// change only the x,y,z
		//Double* updateX = _updateMap->at("name")->s_value().compare(_originalDataMap.at("name")->s_value()) == 0) ? /* do nothing */ : new Double(_upateMap.at("name")->s_value());
		//_originalDataMap.emplace("x", new Double(updateX));
		//Double* updateY = new Double(_originalDataMap.at("y"));
		//_originalDataMap.emplace("y", new Double(updateY));

		for (auto mapIterator = _updateMap.begin(); mapIterator != _updateMap.end(); ++mapIterator)
		{
			if (mapIterator->second->isA(STRING_TYPE))
				_originalDataMap->at(mapIterator->first) = new String(mapIterator->second);
			else if (mapIterator->second->isA(DOUBLE_TYPE))
				_originalDataMap->at(mapIterator->first) = new Double(mapIterator->second);
			else if (mapIterator->second->isA(BOOLEAN_TYPE))
				_originalDataMap->at(mapIterator->first) = new Boolean(mapIterator->second);
		}
	};

	VComponent::VCType VComponent::getVCType() { return Test_Avatar; };

	/* Returns the string name of the internal map.*/
	string getName()
	{
		return _originalDataMap->at("name")->s_value();
	}

	/* Returns the double x of the internal map.*/
	double getX()
	{
		double value;
		Double* vtype = new Double(_originalDataMap->at("x"));
		value = vtype->value();
		delete vtype;
		return value;
	}

private:
	dataMap* _originalDataMap;
}; // end of Avatar class

/* Mock_Up_Test - Tests that the VComponents can work in conjunction with the update map system.
Procedure: Create a VComponent type with an internal map.  Call update on that VComponent, give new values through the update map.  Test those values changed using special getter functions in the VComponent.
Pass: All tested values are updated from the original.
Fail: Some value is not updated or conversion has had a problem.
*/
TEST(Test_VType, Mock_Up_Test)
{
	Avatar* mock_AV = new Avatar("MockAV", 0,0,0, true);
	Avatar* mock_Environment = new Avatar("MockEnvironment", 0,0,0, true);
	Avatar* mock_Obstacle = new Avatar("MockObstacle", 1,1,1, false );
	Avatar* mock_Sensor = new Avatar("MockSensor", 0,0,0, true);
	// the EventTree component

	// External component listener receives new data, updates Avatar's data map from inside
	// initial map values
	dataMap updateMap;
	updateMap.emplace("name", new String("MockChanged"));
	updateMap.emplace("x", new Double(1.0));
	updateMap.emplace("y", new Double(1.0));
	updateMap.emplace("z", new Double(1.0));
	updateMap.emplace("fact", new Boolean(false));

	// Avatar adds an event with this new data to EventTree

	// EventTree disseminates upates to all Avatars but the source of the update
	mock_Environment->update(0.1, updateMap);

	ASSERT_EQ(mock_Environment->getName(), "MockChanged");
	ASSERT_EQ(mock_Environment->getX(), 1.0);
	
	// cleanup
	updateMap.clear();
	delete mock_AV, mock_Environment, mock_Obstacle, mock_Sensor;
}