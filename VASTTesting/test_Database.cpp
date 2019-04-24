//#include "Database.cpp"
#include "DatabaseEventTree.h"
#include "gtest/gtest.h"




static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

/*testing create table function*/
TEST(DatabaseTest, opendatabase_createtable_insertdata_showdata)
{
	
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	int rc1;
	int rc2;
	const char *sql;
	const char **databasename;
	databasename = new const char *("test1.db");
	int test1;

	//testing opendatabase function
	
	test1 = sqlite3_open(*databasename, &db);

	EXPECT_TRUE(true);
	delete databasename;

	//test create table
	sql = "CREATE TABLE AV_Info(run_ID integer(10), AV_ID integer(10), AV_Name varchar(20));";
	

	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	EXPECT_EQ(rc, SQLITE_OK);
	//test insertdata
	sql = "INSERT INTO AV_Info(run_ID, AV_ID, AV_Name)\
		VALUES(1, 1, 'pepper');";
	
	rc1 = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	EXPECT_EQ(rc1, SQLITE_OK);
	//test showdata
	sql = "SELECT * from AV_Info";
	rc2 = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	EXPECT_EQ(rc2, SQLITE_OK);
}


