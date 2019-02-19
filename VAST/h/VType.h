#pragma once
#include <string>

using namespace std;

/*overall class for all variable types*/
class VType
{
public: 
	VType() {};
};

/*class to handle various number types
inherits from VType*/
class NumberType : public VType
{
public:
	NumberType() {};
	virtual bool equals(VType other) = 0;
};

/*class for string data
inherits from VType*/
class String : public VType
{
public:
	/*default constructor
	initializes all values to blank strings*/
	String() { name = ""; val = ""; }

	/*creates a new String type based on a string name and value*/
	String(std::string _name, std::string newVal)
	{
		name = _name;
		val = newVal;
	}

	/*sets the name of the variable of string type*/
	void setName(std::string _name)
	{
		name = _name;
	}

	/*sets the value of the string*/
	void setVal(std::string newVal)
	{
		val = newVal;
	}

	// overload the = operator

	// overload the + operator

	// overload the [] operator

	// overload the << operator
	
	// overload the >> operator

	// overload the == operator

	// overload the != operator

private:
	/*variable to store the name of the string*/
	std::string name;

	/*variable to store the value of the string*/
	std::string val;
};

/*class for integer data
inherits from VType*/
class Integer : public NumberType
{
public:
	/*default constructor
	initializes the name to a blank string and the value to zero*/
	Integer() { name = ""; val = 0; }

	/*creates a new Integer type based on an string name and integer value*/
	Integer(std::string _name, int newVal)
	{
		name = _name;
		val = newVal;
	}

	/*sets the name of the variable of integer type*/
	void setName(std::string _name)
	{
		name = _name;
	}

	/*sets the value of the integer*/
	void setVal(int newVal)
	{
		val = newVal;
	}

private:
	/*variable to store the name of the integer*/
	std::string name;

	/*variable to store the value of the integer*/
	int val;
};

class Double : public NumberType
{
public:
	/*default constructor
	initializes the name to a blank string and the value to zero*/
	Double() { name = ""; val = 0; }

	/*creates a new Double type based on an string name and double value*/
	Double(std::string _name, double newVal)
	{
		name = _name;
		val = newVal;
	}

	/*sets the name of the variable of string type*/
	void setName(std::string _name)
	{
		name = _name;
	}

	/*sets the value of the double*/
	void setVal(double newVal)
	{
		val = newVal;
	}

private:
	/*variable to store the name of the double*/
	std::string name;

	/*variable to store the value of the double*/
	double val;
};

class Boolean : public VType
{
public:
	/*default constructor
	initializes the name to a blank string and the value to true*/
	Boolean() { name = ""; val = true; }

	/*creates a new Boolean type based on an string name and boolean value*/
	Boolean(std::string _name, bool newVal)
	{
		name = _name;
		val = newVal;
	}

	/*sets the name of the variable of string type*/
	void setName(std::string _name)
	{
		name = _name;
	}

	/*sets the value of the boolean*/
	void setVal(bool newVal)
	{
		val = newVal;
	}

private:
	/*variable to store the name of the boolean*/
	std::string name;

	/*variable to store the value of the boolean*/
	bool val;
};