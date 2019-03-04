#include <vector>
#include "pch.h"
#include "..\VAST\h\VType.h"

using std::cout;
using std::vector;

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
	ASSERT_TRUE(vTypes.at(0)->type.compare(STRING_TYPE) == 0);

	// Object 1 - Double
	Double* d = new Double;
	vTypes.push_back(d);
	ASSERT_TRUE(vTypes.at(1)->type.compare(DOUBLE_TYPE) == 0);

	// Object 2 - Integer
	Integer* i = new Integer;
	vTypes.push_back(i);
	ASSERT_TRUE(vTypes.at(2)->type.compare(INTEGER_TYPE) == 0);

	// Object 3 - Boolean
	Boolean* b = new Boolean;
	vTypes.push_back(b);
	ASSERT_TRUE(vTypes.at(3)->type.compare(BOOLEAN_TYPE) == 0);

	// Object 4 - Vector3
	Vector3* v = new Vector3;
	vTypes.push_back(v);
	ASSERT_TRUE(vTypes.at(4)->type.compare(VECTOR3_TYPE) == 0);

	// Object 5 - Array
	Array* a = new Array;
	vTypes.push_back(a);
	ASSERT_TRUE(vTypes.at(5)->type.compare(ARRAY_TYPE) == 0);
	EXPECT_DEATH(delete a, "Wrong message.");

	vTypes.clear();
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
	EXPECT_DEATH(delete a_obj, "Wrong message.");

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

	// Array default is size 1, String with empty string "" value
	Array* testDestructorFirst = new Array;
	// this next EXPECT_DEATH should Fail: until the destructor is properly implemented.
	EXPECT_DEATH(delete testDestructorFirst, "Wrong message.");
	Array* a = new Array;
	EXPECT_TRUE(a->value().compare("") == 0);
	// now change the value in the array
	string test6 = "1,2,3,4,5,6";
	a->value(test6);
	EXPECT_FALSE(a->value().compare("") == 0);
	EXPECT_TRUE(a->value().compare(test6) == 0);
	EXPECT_TRUE(a->value().compare("1,2,3,4,5,6") == 0);	

	// cleanup
	delete s, d, i, v;
	EXPECT_DEATH(delete a, "Wrong message.");

}

/* SpecialStringFunctions - Tests that each special String function operates as predicted.  These include a length() accessor and a concat(...) mutator.
Procedure: For length()	create two strings and set a String object by one of them.  Check the length.  Set the String object by the other string value.  Check the length.  Set the String object to an empty string.  Check the length. For concat(...)	using the original String, call concat and then verify that the return is not a nullptr, and that the new value of String is the combined string values.
Pass: Functions length and concat are implemented correctly.
Fail: Functions length and concat are not implemented correctly, and/or a new String is not returned from concat.
*/
TEST(Test_VType, SpecialStringFunctions)
{
	// Testing length //
	string length1 = "l";
	string length5 = "12345";
	String* s = new String(length1);
	ASSERT_TRUE(s->length() == length1.length());
	// now change the value and test length again
	s->value(length5);
	ASSERT_TRUE(s->length() == length5.length());
	// now set the value to "" and test the length
	ASSERT_TRUE((s->value(""))->length() == 0);

	// Testing Concatenation with string
	// the value of s is reset to "12345"
	EXPECT_TRUE(s->value("12345")->value().compare(length5) == 0);
	// reuse length1 to concatenate s - twice with "1", so "11"
	EXPECT_TRUE((s->concat(length1)) != nullptr);
	EXPECT_TRUE((s->concat(length1))->value().compare("1234511") == 0);

	// Testing Concatenation with integer
	// the value of s is currently "1234511"
	EXPECT_TRUE((s->concat(6)) != nullptr);
	EXPECT_TRUE((s->concat(6))->value().compare("123451166") == 0);
	
	// Testing Concatenation with double
	// the value of s is currently "123451166"
	EXPECT_TRUE((s->concat(7.777)) != nullptr);
	EXPECT_TRUE((s->concat(8.8))->value()
		.compare("1234511667.7778.8") == 0);
	
	// cleanup of pointers
	delete s;
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
	EXPECT_TRUE(d4->value() == 2.0);
	
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
	EXPECT_TRUE(i4->value() == 2);
	
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
	bool falsey = false;
	EXPECT_TRUE(b1->value() == false); // double-checked false
	// result of false(Boolean) OR truth(bool) is true bool
	EXPECT_TRUE(b1 || truthy == true);
	Boolean* b2 = new Boolean;
	// result of false(Boolean) OR false(bool) is false bool
	EXPECT_TRUE(b1 || falsey == false);
	// result of true(bool) OR false(Boolean) is true(bool)
	EXPECT_TRUE((truthy || b2) == true);
	// result of false(Boolean) OR false(Boolean) is false bool
	EXPECT_TRUE((b1 || b2) == false);


	// Testing && comparison - mixing Boolean and bool types to get bool result
	EXPECT_TRUE(b1->value() == false); // still false
	// result of false(Boolean) AND truth(bool) is false(bool)
	EXPECT_TRUE(b1 && truthy == false);
	// result of false(Boolean) AND false(Boolean) is false
	EXPECT_TRUE((b1 && b2) == false);
	// next set b1 and b2 to true 
	b1->value(true);
	b2->value(true);
	// result of true(bool) AND true(Boolean) is true (bool)
	EXPECT_TRUE((truthy && b2) == true);
	// test true(Boolean) AND true(bool) is true(bool)
	EXPECT_TRUE((b1 && truthy) == true);
	// test true(Boolean) AND true(Boolean) is true(bool)
	EXPECT_TRUE((b1 && b2) == true);
	

	// cleanup
	delete b1, b2;
}

/* SpecialArrayFunctions - Tests that the Array special functions operate as predicted.
Procedure: For the destructor function, create an empty Array, add a VType, then destroy the array and verify that the VType object was also destroyed by checking that it was set to nullptr.  
For 
Pass:
Fail: 
*/
TEST(Test_VType, SpecialArrayFunctions)
{
	// Tests Array destructor
	Array* a = new Array();
	Integer* i = new Integer(1);
	a->add(i);
	EXPECT_DEATH(delete a, "Wrong message");
	// it should be true that if a is destroyed, i is also destroyed.
	// if this test fails, i was not destroyed with a.
	EXPECT_TRUE(i == nullptr ? true : emergencyCleanup(i));
	
	// Tests getStowType by testing each Array constructor
	Array* a1 = new Array("one");
	Array* a2 = new Array(2);
	Array* a3 = new Array(3, INTEGER_TYPE); // no set values
	VType* d_array = new Double[4];
	Array* a4 = new Array(4, d_array);
	delete [] d_array; // no longer needed
	vector<VType*> b_vector;
	b_vector.push_back(new Boolean(true)); // 0 index
	b_vector.push_back(new Boolean(false));// 1 
	b_vector.push_back(new Boolean(true)); // 2
	b_vector.push_back(new Boolean(false));// 3
	b_vector.push_back(new Boolean(true)); // 4
	Array* a5 = new Array(b_vector);
	b_vector.clear(); // no longer needed
	EXPECT_TRUE(a1->getStowType().compare(STRING_TYPE) == 0);
	EXPECT_TRUE(a2->getStowType().compare(STRING_TYPE) == 0);
	EXPECT_TRUE(a3->getStowType().compare(INTEGER_TYPE) == 0);
	EXPECT_TRUE(a4->getStowType().compare(DOUBLE_TYPE) == 0);
	EXPECT_TRUE(a5->getStowType().compare(BOOLEAN_TYPE) == 0);

	// Tests setStowType
	EXPECT_TRUE(a1->setStowType(VECTOR3_TYPE)); // won't work
	EXPECT_FALSE(a1->getStowType().compare(VECTOR3_TYPE) == 0); 
	EXPECT_TRUE(a1->getStowType().compare(STRING_TYPE) == 0);
	// try to convert the whole array to a type that doesn't mesh
	EXPECT_FALSE(a1->setStowType(BOOLEAN_TYPE));	
	// add the unlike type anyway (at slot 2)
	Boolean* addB = new Boolean(true);
	a1->add(addB); 
	// the type should have automatically set to VType again
	EXPECT_TRUE(a1->getStowType().compare(V_TYPE) == 0);
	// setting the type to String should always work
	EXPECT_TRUE(a1->setStowType(STRING_TYPE));
	
	// Tests array size function
	EXPECT_TRUE(a1->arraySize() == 2);
	EXPECT_TRUE(a2->arraySize() == 2);
	EXPECT_TRUE(a3->arraySize() == 3);
	EXPECT_TRUE(a4->arraySize() == 4);
	EXPECT_TRUE(a5->arraySize() == 5);

	// Tests index operator
	EXPECT_TRUE(a1->at_String(1)->value().compare("true") == 0);
	EXPECT_TRUE(a2->at_VType(1) == nullptr);
	EXPECT_TRUE(a5->at_Boolean(4)->value() == true);
	
	// cleanup
	delete a1, a2, a3, a4, a5;
	EXPECT_TRUE(addB == nullptr ? true : emergencyCleanup(addB));
}