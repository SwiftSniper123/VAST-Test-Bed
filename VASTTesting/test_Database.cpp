#include "Database.cpp"
#include "DatabaseEventTree.h"
#include "VType.h"
#include "sqlite3.h"
#include "gtest/gtest.h"


/*testing opendatabase function*/
TEST(DatabaseTest, opendatabase)
{
	const char **databasename;
	databasename=new const char *("test1.db");
	int test1;
	sqlite3* db;
	test1= sqlite3_open(*databasename, &db);

	EXPECT_TRUE(test1);
	delete databasename;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

/*testing create table function*/
TEST(DatabaseTest, createtable_insertdata_showdata)
{
	
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char *sql;
	

	//test create table

	sql = "CREATE TABLE RUN_DATA("  \
		"SIM_ID INT PRIMARY KEY     NOT NULL," \
		"RUN_ID         INT     NOT NULL,"
		"TIME_STEP      FLOAT    NOT NULL," \
		"VECH_ID            INT     NOT NULL," \
		"VECH_X        FLOAT," \
		"VECH_Y        FLOAT," \
		"VECH_Z        FLOAT," \
		"VECH_ANGLE        FLOAT," \
		"VECH_TYPE        CHAR(20)," \
		"VECH_SPEED        INT," \
		"VECH_SLOPE    FLOAT);";
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	EXPECT_EQ(rc, SQLITE_OK);
	//test insertdata
	sql = "INSERT INTO"\
		" RUN_DATA (SIM_ID,RUN_ID,TIME_STEP,VECH_ID,VECH_X,VECH_Y,VECH_Z,VECH_ANGLE,VECH_TYPE,VECH_SPEED,VECH_SLOPE) "  \
		"VALUES (1,1,0.02,1,0,0,0,90,'SUV',10,2);" ;
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	EXPECT_EQ(rc, SQLITE_OK);
	//test showdata
	sql = "SELECT * from RUN_DATA";
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	EXPECT_EQ(rc, SQLITE_OK);
}


