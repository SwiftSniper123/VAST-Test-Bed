//#include "Database.cpp"
#include "VC_HEADERS.h"
#include "gtest/gtest.h"

using namespace VASTConstants;

// persistent values that can be verified in every test
static char** colNames = nullptr;
static char** colValues = nullptr;
static int callbackRow = 0;
static int checkRow = 0;

/* Definition of a local SetUp function for each Test case.*/
class DatabaseTest : public ::testing::Test {
protected:
	virtual void SetUp() 
	{ 
		colNames = new char*[3];
		colValues = new char*[3];
		colNames[0] = "run_ID";
		colNames[1] = "AV_ID";
		colNames[2] = "AV_name";
		colValues[0] = "1";
		colValues[1] = "1";
		colValues[2] = "pepper";

		callbackRow = 0;
		checkRow = 0;
	}

	virtual void TearDown()
	{
		delete[] colNames;
		delete[] colValues;
	}
};

// a function provided as a parameter to the sqlite3 execution function that brings us the data from the database
static int callback(void * data, int argc, char **argv, char **azColName) 
{	
	++callbackRow;
	EXPECT_EQ(argc, 3);
	int i;
	if (callbackRow == checkRow)
	{
		for (i = 0; i < argc; i++)
		{
			EXPECT_EQ(*azColName[i], *colNames[i]);
			EXPECT_EQ(*argv[i], *colValues[i]);			
		}
	}
	return 0;
}



/*testing create table function*/
TEST_F(DatabaseTest, opendatabase)
{
	sqlite3 *db;
	const char *fileName = TEST_DATABASE_FILENAME;

	//testing opening database function
	EXPECT_EQ(sqlite3_open(fileName, &db), SQLITE_OK);

	//testing closing the database connection
	EXPECT_EQ(sqlite3_close(db), SQLITE_OK);

	// delete the database; upon success, the return is 0
	EXPECT_EQ(remove(fileName), 0);
}

TEST_F(DatabaseTest, createtable)
{
	sqlite3 *db;
	const char *sql;
	char *zErrMsg = 0;
	const char *fileName = TEST_DATABASE_FILENAME;

	EXPECT_EQ(sqlite3_open(fileName, &db), SQLITE_OK);

	//test create table
	sql = "CREATE TABLE AV_Info(run_ID integer(10), AV_ID integer(10), AV_Name varchar(20));";
	EXPECT_EQ(sqlite3_exec(db, sql, callback, 0, &zErrMsg), SQLITE_OK);

	//testing closing the database connection
	EXPECT_EQ(sqlite3_close(db), SQLITE_OK);

	// delete the database; upon success, the return is 0
	EXPECT_EQ(remove(fileName), 0);

}

TEST_F(DatabaseTest, insertdata)
{
	sqlite3 *db;
	const char *sql;
	char *zErrMsg = 0;
	const char *fileName = TEST_DATABASE_FILENAME;

	EXPECT_EQ(sqlite3_open(fileName, &db), SQLITE_OK);

	sql = "CREATE TABLE AV_Info(run_ID integer(10), AV_ID integer(10), AV_Name varchar(20));";
	EXPECT_EQ(sqlite3_exec(db, sql, callback, 0, &zErrMsg), SQLITE_OK);

	//test insertdata
	sql = "INSERT INTO AV_Info(run_ID, AV_ID, AV_Name)\
		VALUES(1, 1, 'pepper');";

	EXPECT_EQ(sqlite3_exec(db, sql, callback, 0, &zErrMsg), SQLITE_OK);

	EXPECT_EQ(sqlite3_close(db), SQLITE_OK);

	EXPECT_EQ(remove(fileName), 0);
}

TEST_F(DatabaseTest, showdata)
{
	sqlite3 *db;
	stringstream ss;
	char *zErrMsg = 0;
	const char *fileName = TEST_DATABASE_FILENAME;
	string testTable = "AV_Info";

	EXPECT_EQ(sqlite3_open(fileName, &db), SQLITE_OK);

	ss << "CREATE TABLE IF NOT EXISTS " << testTable << 
			"(" << colNames[0] << " integer(10)" << // run_ID
			", " << colNames[1] << " integer(10)"
			", " << colNames[2] << " varchar(20)); ";
	EXPECT_EQ(sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg), SQLITE_OK);
	ss.str("");

	ss << "INSERT INTO " << testTable << "(" << 
			colNames[0] << "," << colNames[1] << ", " << colNames[2] << ") " <<
			"VALUES(" << 
			colValues[0] << ", " << colValues[1] << ", '" << colValues[2] << "'); ";
	EXPECT_EQ(sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg), SQLITE_OK);
	ss.str("");

	//test showdata
	ss << "SELECT * from " << testTable << "; ";
	EXPECT_EQ(sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg), SQLITE_OK);
	ss.str("");

	EXPECT_EQ(sqlite3_close(db), SQLITE_OK);

	EXPECT_EQ(remove(fileName), 0);	
}

TEST_F(DatabaseTest, replications)
{
	sqlite3 *db;
	stringstream ss;
	char *zErrMsg = 0;
	const char *fileName = TEST_DATABASE_FILENAME;
	string testTable = "AV_Info";

	EXPECT_EQ(sqlite3_open(fileName, &db), SQLITE_OK);

	ss << "CREATE TABLE IF NOT EXISTS " << testTable <<
		"(" << colNames[0] << " integer(10)" << // run_ID
		", " << colNames[1] << " integer(10)"
		", " << colNames[2] << " varchar(20)); ";
	EXPECT_EQ(sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg), SQLITE_OK);
	ss.str("");

	ss << "INSERT INTO " << testTable << "(" <<
		colNames[0] << "," << colNames[1] << ", " << colNames[2] << ") " <<
		"VALUES(" <<
		colValues[0] << ", " << colValues[1] << ", '" << colValues[2] << "'); ";
	EXPECT_EQ(sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg), SQLITE_OK);
	ss.str("");

	ss << "SELECT * from " << testTable << "; ";
	EXPECT_EQ(sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg), SQLITE_OK);
	ss.str("");
	callbackRow = 0; // reset the number of callbacks because there was only one row added for that insert

	// testing another "replication" with a new runID
	colValues[0] = "2"; // new run_ID value is 2
	checkRow = 2;  // check the second callback call, which gives data for the second row
	ss << "CREATE TABLE IF NOT EXISTS " << testTable <<
		"(" << colNames[0] << " integer(10)" <<
		", " << colNames[1] << " integer(10)"
		", " << colNames[2] << " varchar(20)); "; // go through the motions of "create"
	EXPECT_EQ(sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg), SQLITE_OK);
	ss.str("");

	ss << "INSERT INTO " << testTable << "(" <<
		colNames[0] << "," << colNames[1] << ", " << colNames[2] << ") " <<
		"VALUES(" << // now insert new column[0] value below
		colValues[0] << ", " << colValues[1] << ", '" << colValues[2] << "'); "; 
	EXPECT_EQ(sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg), SQLITE_OK);
	ss.str("");

	//test that the predicted value is correct
	ss << "SELECT * from " << testTable << "; ";
	EXPECT_EQ(sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg), SQLITE_OK);
	ss.str("");

	EXPECT_EQ(sqlite3_close(db), SQLITE_OK);

	EXPECT_EQ(remove(fileName), 0);
}
