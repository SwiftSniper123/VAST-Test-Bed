#pragma once
//#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cassert> // this can go away when the destructor for Array works properly

using std::string;
using std::ostream;
using std::stringstream;
using std::vector;

static const string V_TYPE = "VType";
static const string STRING_TYPE = "String";
static const string INTEGER_TYPE = "Integer";
static const string DOUBLE_TYPE = "Double";
static const string BOOLEAN_TYPE = "Boolean";
static const string VECTOR3_TYPE = "Vector3";
static const string ARRAY_TYPE = "Array";

/*Overall class for all data types in VAST.
This object's type is V_TYPE, a static const string 
variable public to users of VType.h.
*/
class VType
{
public: 
	VType() { type = V_TYPE; };

	string type;
protected:
	string stringValue;
};

/*Class for string data inherits from VType.
This object's type is STRING_TYPE, a static const string 
variable public to users of VType.h.
*/
class String : public VType
{
private:
	/*variable to store the value of the string*/
	string val;
public:
	/*default constructor
	initializes all values to blank strings*/
	String() : VType()
	{ 
		/*this->val = ""; 
		VType::type = STRING_TYPE;*/
	};

	String(VType& obj) : VType()
	{

	}

	/* Setter constructor initializes to passed string*/
	String(string setter) : VType()
	{
		/*val = setter;
		VType::type = STRING_TYPE;*/
	};

	String::String(String& old_str)
	{
		//val = old_str.value();
	};

	string value()
	{
		return ""; // this->val;
	};

	String* value(string newVal)
	{
		//val = newVal;
		return this;
	};

	/* Special String type Functions*/
	
	/* Returns the length of the String value*/
	int length()
	{
		return 0; //0 strlen(val.c_str());
	};

	/*Concatenation with + string*/
	String* concat(const string appended)
	{
		return nullptr; // String(val + appended);
	};

	/*Concatenation with + String*/
	String* concat(const String appendedVS)
	{
		return nullptr; // String(val + appended);
	};

	/*Concatenation with + int*/
	String* concat(const int appended)
	{
		return nullptr; // String(val + appended);
	};

	/*Concatenation with + double*/
	String* concat(const double appended)
	{
		return nullptr; // String(val + appended);
	};

	//bool equals(VType other)
	//{
	//	bool result = false; // until verified
	//	if (other.type.compare(type) == 0)
	//	{
	//		// since we know this is a String type, we can cast it to compare 
	//		// values
	//		String* childType = dynamic_cast<String*>(other);
	//		if (childType->value().compare(val) == 0)
	//		{
	//			result = true;
	//		}
	//	}
	//	return false;
	//};	
	///*Creates output stream for String data.*/
	//friend ostream& operator<<(std::ostream& out, const String &vdata)
	//{
	//	out << vdata.val;
	//	return out;
	//};
	// Ideas for other possibly useful member functions for String:
	//--------------------------------------
	// overload the - operator to substring
	// overload the [] operator to grab a single char
	// overload the << operator	to print as a ostream
	// overload the >> operator to insert as istream?
	// overload the == operator to compare char
	// overload the != operator
	// create toLowerCase() to make all letters the same
	// create trim() to remove spaces
};

/*Class for Double data inherits from VType.
This object's type is DOUBLE_TYPE, a static const string 
variable public to users of VType.h.
*/
class Double : public VType
{
private:
	double val;
public:
	/*default constructor
	initializes the Double to the value to zero*/
	Double() : VType() 
	{ 
		/*val = 0;
		VType::type = DOUBLE_TYPE;*/
	}

	Double(VType& obj) : VType()
	{

	};

	/*Creates a new Double type based on new double value*/
	Double(double newVal) : VType()
	{
		/*val = newVal;
		VType::type = DOUBLE_TYPE;*/
	}

	/*sets the value of the integer*/
	void value(double newVal)
	{
		//val = newVal;
	}

	/*gets the value of the integer*/
	double value()
	{
		return 0.0; // val;
	};

	/* companion to the operator overload for + below*/
	double operator+=(const double rhs)
	{
		return 0.0;
	};
	
	double operator+=(const Double& rhs)
	{
		return 0.0;
	};

	double Double::operator -=(const Double& otherDouble)
	{
		return 0.0;
	};

	double operator -=(const double other)
	{
		return 0.0;
	};


	/*string format(const Double formatted, int precision)
	{
		return 0;
	}*/
};

/* Special Double arithmetic addition*/
inline double operator+(Double lhs, const double rhs)
{
	return 0.0;
}

inline double operator+(Double lhs, Double rhs)
{
	return 0.0;
}

inline double operator+(const double lhs, Double rhs)
{
	return 0.0;
}

/* Special Double arithmetic subtraction*/
inline double operator-(Double lhs, const double rhs)
{
	return 0.0;
}

inline double operator-(Double lhs, Double rhs)
{
	return 0.0;
}

inline double operator-(const double lhs, Double rhs)
{
	return 0.0;
}

/*Class for integer data inherits from VType.
This object's type is INTEGER_TYPE, a static const string 
variable public to users of VType.h.
*/
class Integer : public VType
{
private: 
	int val;
public:
	/*default constructor
	initializes the Integer to the value to zero*/
	Integer() : VType() 
	{ 
		/*val = 0;
		VType::type = INTEGER_TYPE;*/
	};

	Integer(VType& obj) : VType()
	{

	};

	/*Creates a new Integer type based on new integer value*/
	Integer(int newVal) : VType()
	{
		/*val = newVal;
		VType::type = INTEGER_TYPE;*/
	}

	/*sets the value of the integer*/
	void value(int newVal)
	{
		//val = newVal;
	}

	/*gets the value of the integer*/
	int value()
	{
		return 0; // val;
	}

	/*converts the Integer to Double VType*/
	Double* asDouble()
	{
		return nullptr; // new Double(val);
	};

	/* companions to the operator overload for + below*/
	int operator+=(const int rhs)
	{
		return 0;
	};

	int operator+=(const Integer& rhs)
	{
		return 0;
	};

	int Integer::operator -=(const Integer& otherInteger)
	{
		return 0;
	};

	int operator -=(const int other)
	{
		return 0;
	};
};

/* Special Integer arithmetic addition*/
inline int operator+(Integer lhs, const int rhs)
{
	return 0;
}

inline int operator+(Integer lhs, Integer rhs)
{
	return 0;
}

inline int operator+(const int lhs, Integer rhs)
{
	return 0;
}

/* Special Integer arithmetic subtraction*/
inline int operator-(Integer lhs, const int rhs)
{
	return 0;
}

inline int operator-(Integer lhs, Integer rhs)
{
	return 0;
}

inline int operator-(const int lhs, Integer rhs)
{
	return 0;
}

/*Class for boolean data inherits from VType.
This object's type is BOOLEAN_TYPE, a static const string 
variable public to users of VType.h.
*/
class Boolean : public VType
{
private:
	bool val;
public:
	/*default constructor
	initializes the Boolean to the default value of false*/
	Boolean() : VType() 
	{ 
		/*val = false;
		VType::type = BOOLEAN_TYPE;*/
	};

	Boolean(VType& obj) : VType()
	{

	};

	/*Creates a new Boolean type based on new boolean value*/
	Boolean(bool newVal) : VType()
	{
		/*val = newVal;
		VType::type = BOOLEAN_TYPE;*/
	};

	/*sets the value of the Boolean*/
	void value(bool newVal)
	{
		//val = newVal;
	};

	/*gets the value of the Boolean*/
	bool value()
	{
		return false; // val;
	};

	bool operator||(const bool other)
	{
		return false;
	};

	bool operator||(const Boolean& otherBoolean)
	{
		return false;
	}

	/*Boolean* or (bool other)
	{
		return nullptr;
	}

	Boolean* or (const Boolean* otherBoolean)
	{
		return nullptr;
	}*/

	bool operator&&(const bool other)
	{
		return false;
	}

	bool operator&&(const Boolean& otherBoolean)
	{
		return false;
	}

	/*Boolean* and (bool other)
	{
		return nullptr;
	}

	Boolean* and (const Boolean* otherBoolean)
	{
		return nullptr;
	}*/
};

inline bool operator&&(const bool lhs, Boolean& rhs)
{
	return false;
}

inline bool operator||(const bool lhs, Boolean& rhs)
{

}

/*Class for Vector3 data inherits from VType.
This object's type is VECTOR3_TYPE, a static const string 
variable public to users of VType.h.
*/
class Vector3 : public VType
{
private:
	double _x;
	double _y;
	double _z;
public:
	/*default constructor
	initializes the Vector3 to the default value of 0,0,0*/
	Vector3() : VType() 
	{ 
		/*_x = 0;
		_y = 0;
		_z = 0;
		VType::type = VECTOR3_TYPE;*/
	}

	Vector3(VType& obj) : VType()
	{

	}

	/*Creates a new Vector3 type based on new vector3 values*/
	Vector3(double newx, double newy, double newz) : VType()
	{
		/*_x = newx;
		_y = newy;
		_z = newz;
		VType::type = VECTOR3_TYPE;*/
	}

	/*sets the values of x, y, and z*/
	void value(double newx, double newy, double newz)
	{
		/*_x = newx;
		_y = newy;
		_z = newz;*/
	}

	/* sets the values of x,y,z from a string*/
	void value(string values)
	{

	}

	/*gets the value of the x attribute*/
	double x()
	{
		return 0.0; // _x;
	}

	/*gets the value of the y attribute*/
	double y()
	{
		return 0.0; // _y;
	}

	/*gets the value of the z attribute*/
	double z()
	{
		return 0.0; // _z;
	}
};

/*Class for Array data inherits from VType.
This object's type is ARRAY_TYPE, a static const string 
variable public to users of VType.h.
*/
class Array : public VType
{
private:
	string val; // concatenated string form of all values
	string stowType;
	int size; 
	VType* array;

public:
	/*Default constructor
	initializes the Array to the default value of 1, default type VType, 
	an empty array.*/
	Array() : VType() 
	{ 
		
	}

	/* Downcasting constructor.  If there are no commas in the string value
	of the VType, then this is a VType type Array, arraySize equal to 1.*/
	Array(VType& obj) : VType()
	{

	}

	/*Creates a new Array object based on the parsable string, comma 
	delimited.  Default stowed type is VType.*/
	Array(string parsable) : VType()
	{

	}

	/*Creates a new Array type based on the number to stow, default VType, 
	initializing the array empty of objects.*/
	Array(int size) : VType()
	{
		
	};

	/*Creates a new Array type based on the number to stow, and the type,
	but initializes the array of nullptrs.  Objects must be inserted with
	at_[type](index) function call.  Calls to add(VType) will convert the 
	incoming object to the declared type.  If added or inserted objects 
	cannot be converted to the type, the index will still exist but will be 
	a nullptr.*/
	Array(int size, string type) : VType()
	{

	};

	/*Creates a new Array of VTypes based on copying an existing array 
	of VTypes, 	but can be moved around like a single VType object; if 
	there is more than one VType extension, the stow type remains VType.*/
	Array(int size, VType* objArray) : VType()
	{

	};

	/*Creates an Array from copying a std::vector of VType objects; if 
	there is more than one VType extension, the stow type remains VType.*/
	Array(vector<VType*> objs) : VType()
	{

	};

	/*Destroys all VTypes stowed inside in addition to destroying itself*/
	~Array()
	{
		// until this is implemented properly, tests on Array should not continue
		assert(false); //Creates error message: 
		//"!! This test has probably CRASHED !!"
	}

	/*Sets the value of the Array from a comma delimited string, based 
	upon the declared type; default type is string.
	If the array is not large enough to fit all elements stowed from this new
	value, the values will be stored in a new sized array, and the old array 
	destroyed.  */
	void value(string newVal)
	{
		//val = newVal;
	};

	/*Gets the value of the Array as a comma delimited string*/
	string value()
	{
		return ""; // val;
	};

	/* Adds any VType to the Array.  If the original Array size cannot hold
	more, a new internal array will be dynamically allocated to contain the 
	new amount and the old internal array will be deleted. */
	void add(VType* obj)
	{

	};

	/* Gets the current size of the Array of VTypes.*/
	int arraySize()
	{
		return 0;
	};

	/* Returns the type of VType stowed inside*/
	string getStowType()
	{
		return 0; 
	};

	/* Sets the type of the stowed VTypes in the Array and returns true 
	if this was able to be accomplished; is good for converting numbers 
	and strings back and forth.  SetStowType conversions will not delete 
	the old stowed objects in favor of the new VType.
	Will return false and set will fail if types cannot be converted.  */
	bool setStowType(string type)
	{
		return false;
	}

	/* Returns the pointer to the object at this index in the array.  If the 
	index is out of bounds, or there is no object there, returns nullptr.*/
	VType* at_VType(int index)
	{
		return nullptr;
	}

	String* at_String(int index)
	{
		return nullptr;
	}

	Integer* at_Integer(int index)
	{
		return nullptr;
	};

	Double* at_Double(int index)
	{
		return nullptr;
	};

	Boolean* at_Boolean(int index)
	{
		return nullptr;
	};

	Vector3* at_Vector3(int index)
	{
		return nullptr;
	};
};