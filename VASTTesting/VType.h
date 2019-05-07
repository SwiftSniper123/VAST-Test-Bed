#pragma once
//#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <exception> // this can go away when the destructor for Array works properly

using std::string;
using std::ostream;
using std::stringstream;
using std::vector;
using std::exception;
using std::cerr;
using std::endl;

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
	VType() { this->type = V_TYPE; stringValue = ""; };
	VType(string value) { this->type = V_TYPE; stringValue = value; };
	bool isA(const string otherType) const
	{
		return type.compare(otherType) == 0;
	};
	string getType() const
	{
		return this->type;
	};

	string s_value() const
	{
		return stringValue;
	};
	
	virtual string getSQLite3Text() { return ""; };
protected:
	string stringValue;
	string type;
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
		this->val = ""; 
		VType::stringValue = "";
		VType::type = STRING_TYPE;
	};

	/*Downcasting constructor.  Creates a String object using the stringvalue of the 
	VType input.*/
	String(VType* obj) : VType()
	{
		VType::type = STRING_TYPE;
		VType::stringValue = obj->s_value();
		val = obj->s_value();

		//delete obj; -- let originator do cleanup
	};

	/* Setter constructor initializes to passed string*/
	String(string setter) : VType()
	{
		val = setter;
		VType::stringValue = setter;
		VType::type = STRING_TYPE;
	};

	/* Setter constructor initializes to passed string*/
	String(char* setter) : VType()
	{
		string s(setter);
		val = s;
		VType::stringValue = s;
		VType::type = STRING_TYPE;
	};

	String(String& old_str)
	{
		//val = old_str.value();
	};

	string value()
	{
		return this->val;
	};

	/* Provides string text "VARCHAR(255)" which is an acceptable SQLite3 type.*/
	string getSQLite3Text()
	{
		return "VARCHAR(255)";
	};

	void value(string newVal)
	{
		val = newVal;
		VType::stringValue = newVal;
	};

	/* Special String type Functions*/

	/* Returns the length of the String value*/
	int length()
	{
		return int(val.length()); //0 strlen(val.c_str());
	};

	/*Concatenation with + string*/
	String* concat(const string appended)
	{
		stringstream ss;
		ss << val << appended;
		if (unsigned int(ss.gcount()) < unsigned int(val.max_size()))
		{
			val = ss.str();
			VType::stringValue = val;
		}
		else
		{
			ss.str("");
			// chop off only what will fit
			try
			{
				size_t amtToKeep = appended.length() - ss.gcount() - val.max_size();
				if (amtToKeep < 0)
				{
					throw amtToKeep;
				}
				ss << val << appended.substr(0, amtToKeep); // chop off 
			}
			catch (int amtToKeep)
			{
				cerr << "String object cannot hold any more of concatenated value: {" 
					<< appended << "}" << endl;
			}
			// and then reset the values for this object
			val = ss.str();
			VType::stringValue = val;
		}
		return this; 
	};

	/*Concatenation with String*/
	String* concat(String appendedVS)
	{
		// use a buffer to combine the strings
		stringstream ss;
		ss << val << appendedVS.value();
		// if what is in the buffer is less than string type's maximum size
		if (int(ss.gcount()) <= int(val.max_size()))
		{
			// reset the values for this object
			val = ss.str();
			VType::stringValue = val;
		}
		else
		{
			try
			{
				// otherwise chop off only what will fit
				unsigned int amtToKeep = unsigned int(appendedVS.value().length()) - unsigned int(ss.gcount()) - unsigned int(val.max_size());
				ss.str("");
				if (amtToKeep < 0)
				{
					throw amtToKeep;
				}
				ss << val << appendedVS.value().substr(0, amtToKeep);
			}
			catch (int amtToKeep)
			{
				cerr << "String object cannot hold any more of concatenated value: {"
					<< appendedVS.value() << "}" << endl;
			}
			// and then reset the values for this object
			val = ss.str();
			VType::stringValue = val;
		}
		return this;
	};

	/*Concatenation with + int*/
	String* concat(const int appended)
	{
		// use a buffer to combine the strings
		stringstream ss;
		ss << val << appended;
		// if what is in the buffer is less than string type's maximum size
		if (unsigned int(ss.gcount()) < unsigned int(val.max_size()))
		{
			// reset the values for this object
			val = ss.str();
			VType::stringValue = val;
		}
		else
		{
			ss.str(""); // empty stringstream buffer temporarily
			ss << appended;
			// use stringstream ability to convert integers
			string number = ss.str();
			ss.str("");
			// otherwise chop off only what will fit
			try
			{
				int amtToKeep = unsigned int(number.length()) - unsigned int(ss.gcount()) - unsigned int(val.max_size());
				if (amtToKeep < 0)
				{
					throw amtToKeep;
				}
				ss << val << number.substr(0, amtToKeep); // chop off 
			}
			catch (int amtToKeep)
			{
				cerr << "String object cannot hold any more of concatenated integer: {"
					<< number << "}" << endl;
			}
			// and then reset the values for this object
			val = ss.str();
			VType::stringValue = val;
		}
		return this;
	};

	/*Concatenation with + double*/
	String* concat(const double appended)
	{
		// use a buffer to combine the strings
		stringstream ss;
		ss << val << appended;
		// if what is in the buffer is less than string type's maximum size
		if (unsigned int(ss.gcount()) < unsigned int(val.max_size()))
		{
			// reset the values for this object
			val = ss.str();
			VType::stringValue = val;
		}
		else
		{
			ss.str(""); // empty stringstream buffer temporarily
			ss << appended;
			// use stringstream ability to convert integers
			string number = ss.str();
			ss.str("");
			try
			{
				// otherwise chop off only what will fit
				unsigned int amtToKeep = unsigned int(number.length()) - unsigned int(ss.gcount()) - unsigned int(val.max_size());
				if (amtToKeep < 0)
				{
					throw amtToKeep;
				}
				ss << val << number.substr(0, amtToKeep); // chop off 
			}
			catch (int amtToKeep)
			{
				cerr << "String object cannot hold any more of concatenated double: {"
					<< number << "}" << endl;
			}
			// and then reset the values for this object
			val = ss.str();
			VType::stringValue = val;
		}
		return this;
	};

	/* Compares this String object to the String
	on the right hand side.  Returns true if the 
	values are identical.*/
	bool operator==(String& rhs)
	{
		return rhs.value().compare(val) == 0;
	}

	/* Compares this String object to the string
	on the right hand side.  Returns true if the
	values are identical.*/
	bool operator==(const string rhs)
	{
		return rhs.compare(val) == 0;
	}

	/* Compares this String object to the char*
	on the right hand side.  Returns true if the
	values are identical.*/
	bool operator==(const char* rhs)
	{
		return string(rhs).compare(val) == 0;
	}
	/* Compares this String object to the String
	on the right hand side.  Returns true if the
	values are different.*/
	bool operator!=(String& rhs)
	{
		return rhs.value().compare(val) != 0;
	}

	/* Compares this String object to the string
	on the right hand side.  Returns true if the
	values are different.*/
	bool operator!=(const string rhs)
	{
		return rhs.compare(val) != 0;
	}

	/* Compares this String object to the char*
	on the right hand side.  Returns true if the
	values are different.*/
	bool operator!=(const char* rhs)
	{
		return string(rhs).compare(val) != 0;
	}

	/* Overloaded assignment operator for deep copy*/
	String* operator=(const String& s)
	{

		if (this != &s)   // only copy if the object passed in is not already this one 
		{
			this->type = s.getType();
			this->stringValue = s.s_value();
			this->val = s.s_value();
		}
		return this;     // return the object  
	}
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
// create toLowerCase() to make all letters the same
// create trim() to remove spaces
};

/* Compares a string and String value.  Returns true if identical.*/
inline bool operator==(const string lhs, String& rhs)
{
	return lhs.compare(rhs.value()) == 0;
}

/* Compares a char* and String value.  Returns true if identical.*/
inline bool operator==(const char* lhs, String& rhs)
{
	return string(lhs).compare(rhs.value()) == 0;
}

/* Compares a string and String value.  Returns true if different.*/
inline bool operator!=(const string lhs, String& rhs)
{
	return lhs.compare(rhs.value()) != 0;
}

/* Compares a char* and String value.  Returns true if different.*/
inline bool operator!=(const char* lhs, String& rhs)
{
	return string(lhs).compare(rhs.value()) != 0;
}

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
		val = 0.0;
		VType::stringValue = "0.0";
		VType::type = DOUBLE_TYPE;
	}

	/* Downcasting constructor. Creates a Double object using the stringvalue of the
	VType input.. Can also accept Integers and Strings.*/
	Double(VType* obj) : VType()
	{
		try
		{
			if (obj->isA(DOUBLE_TYPE) ||
				obj->isA(INTEGER_TYPE)||
				obj->isA(STRING_TYPE) ||
				obj->isA(V_TYPE))
			{
				stringstream(obj->s_value()) >> val;
				VType::type = DOUBLE_TYPE;
				VType::stringValue = obj->s_value();
			}			
			else
			{
				throw '!';
			}
		}
		catch (char d)
		{
			cerr << "Double cannot be constructed from type: "
				<< obj->getType() << d << endl;
		}
		catch (exception e)
		{
			cerr << "Double cannot be constructed from: {"
				<< obj->s_value() << "}" << endl;
		}
		//delete obj;  --let the originator do cleanup
	};

	/*Creates a new Double type based on new double value*/
	Double(double newVal) : VType()
	{
		val = newVal;
		stringstream ss;
		ss << newVal;
		VType::stringValue = ss.str();
		VType::type = DOUBLE_TYPE;
	}

	/*sets the value of the double*/
	void value(double newVal)
	{
		val = newVal;
		stringstream ss;		
		ss << newVal;
		VType::stringValue = ss.str();
	}

	/*gets the value of the double*/
	double value()
	{
		return val;
	};

	/* Provides string text "DOUBLE" which is an acceptable SQLite3 type.*/
	string getSQLite3Text()
	{
		return "DOUBLE(10)";
	};

	/* The cumulative addition and assignment operator overloadm, 
	companion to the operator overload for + below.  Can add a double
	to a Double or vice versa.*/
	Double operator+=(const double rhs)
	{
		val += rhs;
		return Double(val);
	};
	
	/* The cumulative addition and assignment operator overloadm,
	companion to the operator overload for + below.  Can add a double
	to a Double or vice versa.*/
	Double operator+=(Double& rhs)
	{
		val += rhs.value();
		return Double(val);
	};

	/* The cumulative subtraction and assignment operator overloadm,
	companion to the operator overload for - below.*/
	Double operator -=(Double& otherDouble)
	{
		val -= otherDouble.value();
		return Double(val);
	};

	/* The cumulative subtraction and assignment operator overloadm,
	companion to the operator overload for - below.*/
	Double operator -=(const double other)
	{
		val -= other;
		return Double(val);
	};

	/* The cumulative division  operator overload,
	Can divide a double with a Double or vice versa.*/
	Double operator/(const double rhs)
	{
		double result = val / rhs;
		return Double(result);
	};

	/* The cumulative division  operator overload,
	Can divide a double with a Double or vice versa.*/
	Double operator/=(const double rhs)
	{
		val /= rhs;
		return Double(val);
	};

	/* The cumulative addition and assignment operator overloadm,
	companion to the operator overload for + below.  Can add a double
	to a Double or vice versa.*/
	Double operator/(const int rhs)
	{
		val += rhs;
		return Double(val);
	};

	/* Overloaded assignment operator for deep copy*/
	Double* operator=(const Double& d)
	{

		if (this != &d)   // only copy if the object passed in is not already this one 
		{
			this->type = d.getType();
			this->stringValue = d.s_value();
			stringstream ss; // ss utility to turn string to double
			ss << d.s_value();
			ss >> this->val;
		}
		return this;     // return the object  
	}

	/* Overloaded assignment operator for less than evaluator*/
	bool operator<(Double& d)
	{
		return this->value() < d.value();     // return the object  
	}

	/* Overloaded assignment operator for greater than evaluator*/
	bool operator>(Double& d)
	{
		return this->value() > d.value();     // return the object  
	}

	/* Overloaded assignment operator for equal to evaluator.  Evaluates to wihin a micro unit*/
	bool operator==(Double& d)
	{
		return abs(this->value() - d.value()) < 0.000000001;     // return the object  
	}

	/* Overloaded assignment operator for equal to evaluator.  Evaluates to wihin a micro unit*/
	bool operator==(double& d)
	{
		return abs(this->value() - d) < 0.000000001;     // return the object  
	}
};

/* Special Double+double addition arithmetic. */
inline double operator+(Double& lhs, const double rhs)
{
	return lhs.value() + rhs;
}

/* Special Double+double addition arithmetic. */
inline double operator+(Double& lhs, Double& rhs)
{
	return lhs.value() + rhs.value();
}

/* Special Double+double addition arithmetic. */
inline double operator+(const double lhs, Double& rhs)
{
	return lhs + rhs.value();
}

/* Special Double subtraction arithmetic */
inline double operator-(Double& lhs, const double rhs)
{
	return lhs.value() - rhs;
}

/* Special Double subtraction arithmetic */
inline double operator-(Double& lhs, Double& rhs)
{
	return lhs.value() - rhs.value();
}

/* Special Double subtraction arithmetic */
inline double operator-(const double lhs, Double& rhs)
{
	return lhs - rhs.value();
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
		val = 0;
		VType::type = INTEGER_TYPE;
		VType::stringValue = "0";

	};

	/* Downcasting constructor. Creates an Integer object using the stringvalue of the
	VType input.  Can also take String inputs.*/
	Integer(VType* obj) : VType()//VType(Integer::stringValue)
	{
		try
		{
			if (obj->isA(INTEGER_TYPE) ||
				obj->isA(STRING_TYPE) ||
				obj->isA(V_TYPE))
			{
				stringstream(obj->s_value()) >> val;
				VType::type = INTEGER_TYPE;
				VType::stringValue = obj->s_value();
			}
			else
			{
				throw '!';
			}
		}
		catch (char d)
		{
			cerr << "Integer cannot be constructed from type: "
				<< obj->getType() << d << endl;
		}
		catch (exception e)
		{
			cerr << "Integer cannot be constructed from: {"
				<< obj->s_value() << "}" << endl;
		}
		//delete obj; -- let originator do cleanup
	};

	/*Creates a new Integer type based on new integer value*/
	Integer(int newVal) : VType()
	{
		val = newVal;
		stringstream ss;
		ss << newVal;
		VType::stringValue = ss.str();
		VType::type = INTEGER_TYPE;
	}

	/*sets the value of the integer*/
	void value(int newVal)
	{
		val = newVal;
		stringstream ss;
		ss << newVal;
		VType::stringValue = ss.str();
	}

	/*gets the value of the integer*/
	int value()
	{
		return val;
	}

	/* Provides string text "INT" which is an acceptable SQLite3 type.*/
	string getSQLite3Text()
	{
		return "INT";
	};

	/*converts the Integer to Double VType*/
	Double* asDouble()
	{
		return new Double(double(val)); // new Double(val);
	};

	/* Compares this Integer object to the Integer
	on the right hand side.  Returns true if the
	values are identical.*/
	bool operator==(Integer& rhs)
	{
		return val == rhs.value();
	}

	/* Compares this Integer object to the int
	on the right hand side.  Returns true if the
	values are identical.*/
	bool operator==(const int rhs)
	{
		return val == rhs;
	}

	/* companions to the operator overload for + below*/
	int operator+=(const int rhs)
	{
		val += rhs;
		return val;
	};

	/* companions to the operator overload for + below*/
	int operator+=(Integer& rhs)
	{
		val += rhs.value();
		return val;
	};

	/* companions to the operator overload for - below*/
	int Integer::operator -=(Integer& otherInteger)
	{
		val -= otherInteger.value();
		return val;
	};

	/* companions to the operator overload for - below*/
	int operator -=(const int other)
	{
		val -= other;
		return val;
	};

	/* Overloaded assignment operator for deep copy*/
	Integer* operator=(const Integer& i)
	{

		if (this != &i)   // only copy if the object passed in is not already this one 
		{
			this->type = i.getType();
			this->stringValue = i.s_value();
			this->val -= i.val;
		}
		return this;     // return the object  
	}
};

/* Special Integer addition arithmetic */
inline int operator+(Integer& lhs, const int rhs)
{
	return lhs.value() + rhs;
}

/* Special Integer addition arithmetic */
inline int operator+(Integer& lhs, Integer& rhs)
{
	return lhs.value() + rhs.value();
}

/* Special Integer addition arithmetic */
inline int operator+(const int lhs, Integer& rhs)
{
	return lhs + rhs.value();
}

/* Special Integer subtraction arithmetic*/
inline int operator-(Integer& lhs, const int rhs)
{
	return lhs.value() - rhs;
}

/* Special Integer subtraction arithmetic*/
inline int operator-(Integer& lhs, Integer& rhs)
{
	return lhs.value() - rhs.value();
}

/* Special Integer subtraction arithmetic*/
inline int operator-(const int lhs, Integer& rhs)
{
	return lhs - rhs.value();
}

/* Compares a string and String value.  Returns true if identical.*/
inline bool operator==(const int lhs, Integer& rhs)
{
	return lhs == rhs.value();
}

/* Compares a string and String value.  Returns true if different.*/
inline bool operator!=(const int lhs, Integer& rhs)
{
	return lhs != rhs.value();
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
		val = false;
		VType::type = BOOLEAN_TYPE;
		VType::stringValue = "false";
	};

	/* Downcasting constructor. Creates a Boolean object using the stringvalue of the
	VType input.*/
	Boolean(VType* obj) : VType()
	{
		try
		{
			if (obj->s_value().compare("true") == 0 
				|| obj->s_value().compare("True") == 0
				|| obj->s_value().compare("TRUE") == 0)
			{
				val = true;
				VType::stringValue = "true";
			}
			else if(obj->s_value().compare("false") == 0
				|| obj->s_value().compare("False") == 0
				|| obj->s_value().compare("FALSE") == 0)
			{
				val = false;
				VType::stringValue = "false";
			}
			else
			{
				throw '!';
			}
		}
		catch (char s)
		{
			cerr << "Boolean cannot be constructed from: {"
				<< obj->s_value() << "}" << s << endl;
		}
		VType::type = BOOLEAN_TYPE;
	};

	/*Creates a new Boolean type based on new boolean value*/
	Boolean(bool newVal) : VType()
	{
		val = newVal;
		if (newVal)
		{
			VType::stringValue = "true";
		}
		else if (!newVal)
		{
			VType::stringValue = "false";
		}		
		VType::type = BOOLEAN_TYPE;
	};

	/*sets the value of the Boolean*/
	void value(bool newVal)
	{
		val = newVal;
		if (newVal)
		{
			VType::stringValue = "true";
		}
		else if (!newVal)
		{
			VType::stringValue = "false";
		}
	};

	/*gets the value of the Boolean*/
	bool value()
	{
		return val;
	};

	/* Provides string text "BOOLEAN" which is an acceptable SQLite3 type.*/
	string getSQLite3Text()
	{
		return "BOOLEAN";
	};

	/* Compares this Boolean object to the bool
	on the right hand side.  Returns true if one
	value is true.*/
	bool operator||(const bool other)
	{
		return (val || other);
	};

	/* Compares this Boolean object to the Boolean
	on the right hand side.  Returns true if one
	value is true.*/
	bool operator||(Boolean& otherBoolean)
	{
		return (val || otherBoolean.value());
	}

	/* Compares this Boolean object to the bool
	on the right hand side.  Returns true if both
	values are true.*/
	bool operator&&(const bool other)
	{
		return (val && other);
	}

	/* Compares this Boolean object to the Boolean
	on the right hand side.  Returns true if both 
	values are true.*/
	bool operator&&(Boolean& otherBoolean)
	{
		return (val && otherBoolean.value());
	}

	/* Compares this Boolean object to the Boolean
	on the right hand side.  Returns true if the
	values are identical.*/
	bool operator==(Boolean& rhs)
	{
		return val == rhs.value();
	}

	/* Compares this Boolean object to the bool
	on the right hand side.  Returns true if the
	values are identical.*/
	bool operator==(const bool rhs)
	{
		return val == rhs;
	}

	/* Overloaded assignment operator for deep copy*/
	Boolean* operator=(const Boolean& b)
	{

		if (this != &b)   // only copy if the object passed in is not already this one 
		{
			this->type = b.getType();
			this->stringValue = b.s_value();
			this->val = b.val;
		}
		return this;     // return the object  
	}
};

/* Compares a bool and Boolean value.  Returns true if both are true.*/
inline bool operator&&(const bool lhs, Boolean& rhs)
{
	return (lhs && rhs.value());
}

/* Compares a bool and Boolean value.  Returns true if one is true.*/
inline bool operator||(const bool lhs, Boolean& rhs)
{
	return (lhs || rhs.value());
}

/* Compares a bool and Boolean value.  Returns true if identical.*/
inline bool operator==(const bool lhs, Boolean& rhs)
{
	return lhs == rhs.value();
}

/* Compares a bool and Boolean value.  Returns true if different.*/
inline bool operator!=(const bool lhs, Boolean& rhs)
{
	return lhs != rhs.value();
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

	/* Breaks up a string of comma delimited items into a Vector3.  Handles 
	size determination*/
	void parseVector(string parsable)
	{
		try {
			short commaCount = 0;
			short len = short(parsable.length());
			for (int j = 0; j < len; j++)
			{
				if (parsable[j] == ',')
				{
					commaCount++;
				}
			}
			if (commaCount > 2)
			{
				throw commaCount;
			}
			stringstream stowable;
			int currentIndex = 0;
			for (int i = 0; i < len; i++) // loop through every character
			{
				if (parsable[i] == '-')
				{
					stowable << parsable[i];
				}
				else
				{
					if (parsable[i] != ',')
					{
						// piece together the value character by character
						stowable << parsable[i];
					}
					else if (parsable[i] == ',' && currentIndex == 0)	// x
					{
						stowable >> _x;
						currentIndex++;
						stowable.str("");
						stowable.clear();
					}
					else if (parsable[i] == ',' && currentIndex == 1)	// y
					{
						stowable >> _y;
						currentIndex++;
						stowable.str("");
						stowable.clear();
					}

					if (currentIndex == 2 && i == len - 1) 				// z
					{
						stowable>> _z;
						stowable.str("");
						stowable.clear();
					}
				}
			}
		}
		catch (int c)
		{
			cerr << "Vector3 cannot be created with " << c 
				<< " elements from the following string: {" << parsable << "}" 
				<< endl;
		}
		catch (exception e)
		{
			cerr << "Vector3 cannot be created with the following string: {" 
				<< parsable << "}" << endl;
		}
	}
public:
	/*default constructor
	initializes the Vector3 to the default value of 0,0,0*/
	Vector3() : VType() 
	{ 
		_x = 0;
		_y = 0;
		_z = 0;
		VType::type = VECTOR3_TYPE;
		stringstream ss;
		ss <<  _x << "," << _y << "," << _z ;
		VType::stringValue = ss.str();
	}

	/*  Downcasting constructor. Creates a Vector3 object using the stringvalue of the
	VType input.*/
	Vector3(VType* obj) : VType()
	{
		parseVector(obj->s_value());
		VType::type = VECTOR3_TYPE;
		VType::stringValue = obj->s_value();
	}

	/*Creates a new Vector3 type from a string assumed to have three comma delimited numericals.  */
	Vector3(string parsable)
	{
		
		parseVector(parsable);
		VType::type = VECTOR3_TYPE;
		VType::stringValue = parsable;
	}

	/*Creates a new Vector3 type based on new vector3 values*/
	Vector3(double newx, double newy, double newz) : VType()
	{
		_x = newx;
		_y = newy;
		_z = newz;
		VType::type = VECTOR3_TYPE;
		stringstream ss;
		ss << _x << "," << _y << "," << _z;
		VType::stringValue = ss.str();
	}

	/*sets the values of x, y, and z*/
	void value(double newx, double newy, double newz)
	{
		_x = newx;
		_y = newy;
		_z = newz;
		stringstream ss;
		ss << _x << "," << _y << "," << _z;
		VType::stringValue = ss.str();
	}

	/* sets the values of x,y,z from a string*/
	void value(string values)
	{
		parseVector(values);
		stringstream ss;
		ss << _x << "," << _y << "," << _z;
		VType::stringValue = ss.str();
	}

	/* gets the value of the entire vector3 as a string*/
	string value()
	{
		return VType::stringValue;
	}

	/*gets the value of the x attribute*/
	double x()
	{
		return _x;
	}

	/*gets the value of the y attribute*/
	double y()
	{
		return _y;
	}

	/*gets the value of the z attribute*/
	double z()
	{
		return _z;
	}

	Vector3* operator-(Vector3* that)
	{
		return new Vector3(this->_x - that->x(),this-> _y - that->y(), this->_z - that->z());
	}

	/* compares two Vector3 objects.  Returns true if the double values are identical.*/
	bool operator==(Vector3& other)
	{
		return _x == other.x() && _y == other.y() && _z == other.z();
	}

	/* compares two Vector3 objects.  Returns true if the double values are different.*/
	bool operator!=(Vector3& other)
	{
		return !(_x == other.x() && _y == other.y() && _z == other.z());
	}

	/* Overloaded assignment operator for deep copy*/
	Vector3* operator=(const Vector3* v)
	{
		if (this != v)   // only copy if the object passed in is not already this one 
		{
			this->type = v->getType();
			this->stringValue = v->s_value();
			parseVector(this->stringValue); // handles x,y,z
		}
		return this;     // return the object  
	}
};

/*Class for Array data inherits from VType.
This object's type is ARRAY_TYPE, a static const string 
variable public to users of VType.h.
*/
class Array : public VType
{
private:
	string stowType; /* the type that describes all of the stowed objects*/
	int size = 0; /* the size of the internal array*/
	VType** stowArray; /* the internal array of VTypes "stowed" inside this object*/

	/* Breaks up a string of comma delimited items into an array of
	VTypes.  Handles size determination*/
	void parseAndStow(string parsable)
	{
		short commaCount = 0;
		short len = short(parsable.length());
		for (int j = 0; j < len; j++)
		{
			if (parsable[j] == ',')
			{
				commaCount++;
			}
		}
		size = commaCount + 1;
		stowArray = new VType*[size];
		string objStringVal = "";
		int currentIndex = 0;
		// loop through all characters, example: "abc,def,ghi"
		for (int i = 0; i < len; i++) // loop through every character
		{
			// reading characters in the middle, like "abc or def or gh
			if (parsable[i] != ',' && i < len - 1)
			{
				// piece together the value character by character
				objStringVal += parsable[i];
			}
			// read in the comma as a place to create a VType
			else if (parsable[i] == ',' && currentIndex < size)
			{
				//objStringVal += parsable[i];
				stowArray[currentIndex] = new String(objStringVal);
				currentIndex++;
				objStringVal = ""; // empty string again
			}
			// the last item is not followed by a comma
			else if (parsable[i] != ',' && i == len - 1)
			{
				objStringVal += parsable[i];
				stowArray[currentIndex] = new String(objStringVal);
			}
		}
	}
public:
	/*Default constructor
	initializes the Array to the default value of 1, default type String Type,
	an empty array.*/
	Array() : VType()
	{
		VType::type = ARRAY_TYPE;
		VType::stringValue = "";
		size = 1;
		stowArray = new VType*[1];
		stowArray[0] = new String();
		stowType = STRING_TYPE;
	}

	/* Downcasting constructor.  Assumes stow type is V_TYPE. If there
	are no commas in the string value of the VType, then this arraySize
	is equal to 1.*/
	Array(VType* obj) : VType()
	{
		if (obj->isA(ARRAY_TYPE))
		{
			Array* arrayObj = (Array*)obj;
			VType::type = ARRAY_TYPE;
			VType::stringValue = obj->s_value();			
			parseAndStow(stringValue); // handles size and stowArray contents
			stowType = arrayObj->getStowType();		
		}
	}

	/*Creates a new Array object based on the parsable string, comma
	delimited.  Default stowed type is String Type.*/
	Array(string _parsable) : VType()
	{
		VType::type = ARRAY_TYPE;
		VType::stringValue = _parsable;
		parseAndStow(_parsable); // handles size and stowArray contents
		stowType = STRING_TYPE;
	}

	/*Creates a new Array type based on the number to stow, default String
	Type, initializing the array of nullptrs.  Objects must be inserted
	with add function call.  */
	Array(int _size) : VType()
	{
		VType::type = ARRAY_TYPE;
		stringstream newStringValue;
		size = _size;
		stowArray = new VType*[size]; // VType slots with no VType objects in them
		for (int i = 0; i < size; i++)
		{
			stowArray[i] = new String();
			if (i < size - 1)
				newStringValue << ",";
		}
		stowType = STRING_TYPE;
		VType::stringValue = newStringValue.str();
	};

	/*Creates a new Array type based on the number to stow, and the type,
	but initializes the array of nullptrs.  Objects must be inserted with
	add function call.  */
	Array(int _size, string _stowtype) : VType()
	{
		VType::type = ARRAY_TYPE;
		stringstream newStringValue;
		size = _size;
		stowArray = new VType*[size]; // VType slots with no VType objects in them
		for (int i = 0; i < size; i++)
		{
			if (_stowtype.compare(STRING_TYPE) == 0)
			{
				stowArray[i] = new String("");
				// no need to add "" to newStringValue
			}
			else if (_stowtype.compare(DOUBLE_TYPE) == 0)
			{
				stowArray[i] = new Double(0.0);
				newStringValue << 0.0;
			}
			else if (_stowtype.compare(INTEGER_TYPE) == 0)
			{
				stowArray[i] = new Integer(0);
				newStringValue << 0;
			}
			else if (_stowtype.compare(BOOLEAN_TYPE) == 0)
			{
				stowArray[i] = new Boolean(false);
				newStringValue << "false";
			}
			else if (_stowtype.compare(VECTOR3_TYPE) == 0)
			{
				stowArray[i] = new Vector3(0);
				newStringValue << "{0.0,0.0,0.0}";
			}
			else if (_stowtype.compare(ARRAY_TYPE) == 0)
			{
				throw std::invalid_argument("Cannot create an Array of Arrays with VTypes.");
			}
			if (i < size - 1)
				newStringValue << ",";
		}
		stowType = _stowtype;
		VType::stringValue = newStringValue.str();
	};

	/*Creates a new Array of VTypes based on making a deep copy of an
	existing array 	of VTypes, but can be moved around like a single VType
	object; if 	there is more than one VType extension, the stow type
	remains VType.  The _objArray parameter must be an array of a child of
	VType.*/
	Array(int _size, VType** _objArray) : VType()
	{
		VType::type = ARRAY_TYPE;
		stringstream newStringValue;
		size = _size;
		string _stowtype = "";
		// VType slots with undefined VType objects in them
		stowArray = new VType*[size];
		try
		{
			int otherIndex = 0;
			if (_objArray != 0)
			{
				for (int i = 0; i < _size; i++)
				{
					otherIndex++; // increment the number of VTypes we discovered
					if (_objArray[i]->isA(STRING_TYPE)) // copy VTypes
					{
						stowArray[i] = new String(_objArray[i]);
					}
					else if (_objArray[i]->isA(DOUBLE_TYPE))
					{
						stowArray[i] = new Double(_objArray[i]);
					}
					else if (_objArray[i]->isA(INTEGER_TYPE))
					{
						stowArray[i] = new Integer(_objArray[i]);
					}
					else if (_objArray[i]->isA(BOOLEAN_TYPE))
					{
						stowArray[i] = new Boolean(_objArray[i]);
					}
					else if (_objArray[i]->isA(VECTOR3_TYPE))
					{
						stowArray[i] = new Vector3(_objArray[i]);
					}
					else if (_objArray[i]->isA(ARRAY_TYPE))
					{
						throw std::invalid_argument("Cannot create an Array of Arrays with VTypes.");
					}
					// set the type is previously unset or the same as this type
					string incomingType = _objArray[i]->getType();
					_stowtype = (_stowtype.compare("") == 0) ||  // if no type is set yet
						(_objArray[i]->isA(_stowtype)) ?	 // or the types are the same
						incomingType :		 // set the new type
						V_TYPE; // or, set to V_TYPE
					
					// if there are 5 objects, only place comma in stringValue up 
							// to the 4th object
					if (i < _size - 1)
					{
						if (_objArray[i]->isA(VECTOR3_TYPE))
						{
							newStringValue << "{" << _objArray[i]->s_value() << "},";
						}
						else
						{
							newStringValue << _objArray[i]->s_value() << ",";
						}
					}
					else if (i == _size - 1 && _objArray[i]->isA(VECTOR3_TYPE))
					{
						newStringValue << "{" << _objArray[i]->s_value() << "}";
					}
				}
			}
			else
			{
				throw '!';
			}
		}
		catch (int t)
		{
			cerr << "Something is wrong with index " << t << " passed in to this Array.  No VType was found." << endl;
		}
		catch (char s)
		{
			cerr << "The array passed to this Array VType is empty" << s << endl;
		}
		stowType = _stowtype;
		VType::stringValue = newStringValue.str();
	};

	/*Creates an Array from copying a std::vector of VType objects; if
	there is more than one VType extension, the stow type remains VType.*/
	Array(vector<VType*> objs) : VType()
	{
		VType::type = ARRAY_TYPE;
		stringstream newStringValue;
		string _stowtype = "";
		size = int(objs.size());
		// VType slots with undefined VType objects in them
		stowArray = new VType*[size];
		try
		{
			int otherIndex = 0;
			if (objs.size() != 0)
			{
				for (int i = 0; i < objs.size(); i++)
				{
					otherIndex++; // increment the number of VTypes we discovered
					stowArray[i] = objs[i];	// copy VTypes
					// set the type is previously unset or the same as this type
					_stowtype = (_stowtype.compare("") == 0) ||  // if no type is set yet
						(objs[i]->isA(_stowtype)) ?	 // or the types are the same
						objs[i]->getType() :		 // set the new type
						V_TYPE; // or, set to V_TYPE

					// if there are 5 objects, only place comma in stringValue up 
					// to the 4th object
					if (i < objs.size() - 1)
					{
						if (objs[i]->isA(VECTOR3_TYPE))
						{
							newStringValue << "{" << objs[i]->s_value() << "},";
						}
						else
						{
							newStringValue << objs[i]->s_value() << ",";
						}
					}
					else if (i == objs.size() - 1 && objs[i]->isA(VECTOR3_TYPE))
					{
						newStringValue << "{" << objs[i]->s_value() << "}";
					}
					
				}
			}
			else
			{
				throw '!';
			}
		}
		catch (int t)
		{
			cerr << "Something is wrong with index " << t << " passed in to this Array.  No VType was found." << endl;
		}
		catch (char s)
		{
			cerr << "The vector passed to this Array VType is empty" << s << endl;
		}
		stowType = _stowtype;
		VType::stringValue = newStringValue.str();
	};

	/*Destroys all VTypes stowed inside in addition to destroying itself*/
	~Array()
	{
		if (size > 0)
		{
			for (int i = 0; i < size; i++)
			{
				delete stowArray[i];
				stowArray[i] = nullptr;
			}
			delete[] stowArray;
			size = 0;
			stowType = "";
		}

	}

	/*Sets the value of the Array from a comma delimited string, based
	upon the declared type; default type is string.
	If the array is not large enough to fit all elements stowed from this new
	value, the values will be stored in a new sized array, and the old array
	destroyed.  */
	void value(string newVal)
	{
		VType::stringValue = newVal;
	};

	/*Gets the value of the Array as a comma delimited string*/
	string value()
	{
		return VType::stringValue;
	};

	/* Adds any VType to the Array.  Replaces the old internal array with a
	new array that will be dynamically allocated to contain the
	new amount and the old internal array will be deleted. If calls to add(VType)
	add a VType object with a different type than the Array stowed type, the
	add operation will convert the Array stowed type to VType.  */
	void add(VType* obj)
	{
		int newSize;
		if (size < 0)
		{
			newSize = 1;
		}
		else
		{
			newSize = size + 1;
		}
		
		stringstream newArrayStringValue;
		VType** newArray = new VType*[newSize];
		// add all old objects from the Array to this new larger array string value
		for (int i = 0; i < size; i++)
		{
			newArray[i] = stowArray[i];
			if (stowType.compare(VECTOR3_TYPE) == 0)
			{
				newArrayStringValue << "{" << stowArray[i]->s_value() << "},";
			}
			else
			{

				newArrayStringValue << stowArray[i]->s_value() << ",";
			}
		}

		// add the object to the newArray at this index
		newArray[newSize - 1] = obj;

		size = newSize;
		delete[] stowArray;
		stowArray = new VType*[newSize];
		// redirect the old array to the new array; objects are the same, so 
		// no deletion is needed
		for (int i = 0; i < newSize; i++)
		{
			stowArray[i] = newArray[i];
			if (stowType.compare(VECTOR3_TYPE) == 0)
			{
				newArrayStringValue << "{" << stowArray[i]->s_value() << "}";
			}
			else
			{

				newArrayStringValue << stowArray[i]->s_value();
			}
		}
		VType::stringValue = newArrayStringValue.str();
		// if the new object type does not match this Array's stowed objects' 
		// type, convert stowType to general VType
		stowType = obj->isA(stowType) ? stowType : V_TYPE;
		delete[] newArray;
	};

	/* Gets the current size of the Array of VTypes.*/
	int arraySize()
	{
		return size;
	};

	/* Returns the type of VType stowed inside*/
	string getStowType()
	{
		return stowType;
	};

	// setStowType NOT CURRENTLY IMPLEMENTED due to constrained use cases, but kept for 
	// the purpose of thoroughness.
	///* Sets the type of the stowed VTypes in the Array and returns true 
	//if this was able to be accomplished; is good for converting numbers 
	//and strings back and forth.  SetStowType conversions will not delete 
	//the old stowed objects in favor of the new VType.
	//Will return false and set will fail if types cannot be converted.  */
	//bool setStowType(string type)
	//{
	//	bool result = false;
	//	if (isCompatable(this->stowType, type))
	//	{
	//		for(int i = 0; i < size; i++)
	//	}
	//	
	//	return result;
	//}

	/* Returns the pointer to the object at this index in the array.  If the
	index is out of bounds, or there is no object there, returns nullptr.*/
	VType* at_VType(int index)
	{
		VType* obj;
		if (index < size)
		{
			obj = stowArray[index];
		}
		else
		{
			obj = nullptr;
		}
		return obj;
	}

	/* Returns the pointer to the object at this index in the array.  If the
	index is out of bounds, or there is no object there, returns nullptr.*/
	String* at_String(int index)
	{
		String* obj;
		if (index < 0)
		{
			stringstream ss;
			ss << "Array cannot search index " << index << "; this does not exist";
			throw std::invalid_argument(ss.str());
		}
		else if (index >= size || stowArray[index] == nullptr)
		{
			obj = nullptr;
		}
		else if (stowArray[index]->isA(STRING_TYPE))
		{
			obj = new String(stowArray[index]);
		}
		else if (stowArray[index]->isA(V_TYPE))
		{
			obj = new String(stowArray[index]->s_value());
		}
		else if (!(stowArray[index]->isA(V_TYPE)))
		{
			stringstream ss;
			ss << "Array index " << index << " does not contain a String." \
				" It contains a " << stowArray[index]->getType();
			throw std::invalid_argument(ss.str());
		}
		else
		{
			stringstream ss;
			ss << "Array index " << index << " cannot convert to a String.";
			throw std::invalid_argument(ss.str());
		}
		return obj;
	}

	/* Returns the pointer to the object at this index in the array.  If the
	index is out of bounds, or there is no object there, returns nullptr.*/
	Integer* at_Integer(int index)
	{
		Integer* obj;
		if (index < 0)
		{
			stringstream ss;
			ss << "Array cannot search index " << index << "; this does not exist";
			throw std::invalid_argument(ss.str());
		}
		else if (index >= size || stowArray[index] == nullptr)
		{
			obj = nullptr;
		}
		else if (stowArray[index]->isA(INTEGER_TYPE))
		{
			obj = new Integer(stowArray[index]);
		}
		else
		{
			stringstream ss;
			ss << "Array index " << index << " cannot convert to an Integer." \
				" It is a " << stowArray[index];
			throw std::invalid_argument(ss.str());
		}
		return obj;
	};

	/* Returns the pointer to the object at this index in the array.  If the
	index is out of bounds, or there is no object there, returns nullptr.*/
	Double* at_Double(int index)
	{
		Double* obj;
		if (index < 0)
		{
			stringstream ss;
			ss << "Array cannot search index " << index << "; this does not exist";
			throw std::invalid_argument(ss.str());
		}
		else if (index >= size || stowArray[index] == nullptr)
		{
			obj = nullptr;
		}
		else if (stowArray[index]->isA(DOUBLE_TYPE))
		{
			obj = new Double(stowArray[index]);
		}
		else
		{
			stringstream ss;
			ss << "Array index " << index << " cannot convert to a Double." \
				" It is a " << stowArray[index];
			throw std::invalid_argument(ss.str());
		}
		return obj;
	};

	/* Returns the pointer to the object at this index in the array.  If the
	index is out of bounds, or there is no object there, returns nullptr.*/
	Boolean* at_Boolean(int index)
	{
		Boolean* obj;
		if (index < 0)
		{
			stringstream ss;
			ss << "Array cannot search index " << index << "; this does not exist";
			throw std::invalid_argument(ss.str());
		}
		else if (index >= size || stowArray[index] == nullptr)
		{
			obj = nullptr;
		}
		else if (stowArray[index]->isA(BOOLEAN_TYPE))
		{
			obj = new Boolean(stowArray[index]);
		}
		else
		{
			stringstream ss;
			ss << "Array index " << index << " cannot convert to a Boolean." \
				" It is a " << stowArray[index];
			throw std::invalid_argument(ss.str());
		}
		return obj;
	};

	/* Returns the pointer to the object at this index in the array.  If the
	index is out of bounds, or there is no object there, returns nullptr.*/
	Vector3* at_Vector3(int index)
	{
		Vector3* obj;
		if (index < 0)
		{
			stringstream ss;
			ss << "Array cannot search index " << index << "; this does not exist";
			throw std::invalid_argument(ss.str());
		}
		else if (index >= size || stowArray[index] == nullptr)
		{
			obj = nullptr;
		}
		else if (stowArray[index]->isA(VECTOR3_TYPE))
		{
			obj = new Vector3(stowArray[index]);
		}
		else
		{
			stringstream ss;
			ss << "Array index " << index << " cannot convert to a Vector3." \
				" It is a " << stowArray[index];
			throw std::invalid_argument(ss.str());
		}
		return obj;
	};

	/* Overloaded assignment operator for deep copy*/
	Array* operator=(const Array* a)
	{
		if (this != a)   // only copy if the object passed in is not already this one 
		{
			this->type = a->getType();
			this->stringValue = a->s_value();
			this->stowType = a->stowType;
			this->size = a->size;
			this->stowArray = new VType*[a->size];
			for (int i = 0; i < this->size; i++)
			{
				this->stowArray[i] = a->stowArray[i];
			}
			delete a;
		}
		return this;     // return the object  
	}
};