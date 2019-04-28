/*Virtual Autonomous System Testbed
ODU Capstone 2018-2019

Database.cpp
Definitions for EventTree functions pertaining specifically to the database creation, table creation, insertion, 
*/
#include "EventTree.h"
#include <typeinfo>

void EventTree::opendatabase(string fileName)
{
	// if fileName is empty, assign a default name
	if (fileName.empty())
	{
		fileName = "VASTdatabase.db";
	}
	rc = sqlite3_open(fileName.c_str(), &db);

	if (!SQLITE_OK) 
	{
		throw DatabaseException("Cannot open. " + string(sqlite3_errmsg(db)));
	}
}

void EventTree::closedatabase()
{
	rc = sqlite3_close(db);

	if (!SQLITE_OK)
	{
		throw DatabaseException("Cannot close. " + string(sqlite3_errmsg(db)));
	}
}

void EventTree::createtable(tableMap* componentsToData, const char* tableType)
{
	/*this is an example of sql statement of creating a table in database
	sql = "CREATE TABLE AV_Bounds (AV_ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, Bound_X double(10), Bound_Y double(10), Bound_Z double(10));"\
		"CREATE TABLE AV_Configuration (Scenario_ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, AV_ID integer(10), AV_Name varchar(20), AV_Config_File varchar(255), AV_Program_File varchar(255), AV_Port integer(10));"\
		"CREATE TABLE AV_Init_Orientation (AV_ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, Orientation_X double(10), Orientation_Y double(10), Orientation_Z double(10));"\
		"CREATE TABLE AV_Init_Position (AV_ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, Pos_X double(10), Pos_Y double(10), Pos_Z double(10));"\
		"CREATE TABLE AV_Map (AV_ID integer(10) NOT NULL, AV_Map_Row_ID integer(10) NOT NULL, AV_Map_Row_Name varchar(20), AV_Map_Row_Data_Type varchar(20), AV_Map_Row_Init_Value varchar(20), PRIMARY KEY (AV_ID, AV_Map_Row_ID));"\
		"CREATE TABLE Env_Configuration (Scenario_ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, Env_ID integer(10), Env_Config_File varchar(255), Env_Program_File varchar(255), Env_Port integer(10), Env_Bounds varchar(255));"\
		"CREATE TABLE Env_Map (Env_ID integer(10) NOT NULL, Env_Map_Row_ID integer(10) NOT NULL, Env_Map_Row_Name varchar(20), Env_Map_Row_Data_Type varchar(20), Env_Map_Row_Init_Value varchar(20), PRIMARY KEY (Env_ID, Env_Map_Row_ID));"\
		"CREATE TABLE VAST_Config (Scenario_ID integer(10) NOT NULL, run_ID integer(10) NOT NULL, viz_Option integer(1), time_Setting float(10), time_Step float(10), max_Sim_Time float(10), replications integer(10), seed integer(10), sensor_List varchar(255), metric_List varchar(255), PRIMARY KEY (Scenario_ID));";*/

	//sql = statement.c_str;	

	stringstream statement1;
	stringstream statement2;
	stringstream statement3;
	stringstream statement4;

	for (auto component = componentsToData->begin(); component != componentsToData->end(); ++component)
	{
		    
		/*this part is to print the first column name and value type
			for example : AV_ID INTEGER NOT NULL PRIMARY KEY*/
		auto componentDataMap = component->second.begin();
			
		statement1 << componentDataMap->first << " " << componentDataMap->second->getSQLite3Text() 
			<< " NOT NULL PRIMARY KEY, ";
			
		for (componentDataMap; componentDataMap != avmap->end(); ++componentDataMap)
		{
			/*this part is to print the rest column name and value type*/
			statement2 << componentDataMap->first << " " << component->second->getSQLite3Text() << " NOT NULL, ";
		}
		/*print first and rest column name and value type*/
		statement3 << statement1.str() << statement2.str();
	    /*this is print all sql statement*/
		statement4 << " CREATE TABLE IF NOT EXISTS "<< component->first->getName() << tableType 
			<< " ('" << statement3.str() << "');";
	}

	/*change stringstream to const char*/
	sql = statement4.str().c_str();

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		throw DatabaseException();
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Table created successfully\n");
	}

}
void EventTree::publishEvent(VComponent* source, timestamp time, dataMap *tablemap, dataMap *avmap)
//void database::insertdata()
{
	stringstream statement1;
	stringstream statement2;
	stringstream statement3;

	for (auto mapIterator = tablemap->begin(); mapIterator != tablemap->end(); ++mapIterator)
	{
		for (auto mapIterator2 = avmap->begin(); mapIterator2 != avmap->end(); ++mapIterator2)
		{
			/*print column name , example: SIM_ID,RUN_ID,TIME_STEP,VECH_ID,VECH_X,VECH_Y,VECH_Z,VECH_ANGLE,VECH_TYPE,VECH_SPEED,VECH_SLOPE */
			statement1 << statement1.str() << "," << mapIterator2->first;
			/*print valuse, example: 1,1,0.02,1,0,0,0,90,'SUV',10,2*/
			statement2 << statement2.str() << "," << mapIterator2->second;
		}
		/*print full sql statement*/
		statement3 << "INSERT INTO "<< mapIterator->first<<"(" << statement1.str() << ") VALUES (" << statement2.str() << ");";
	}
	
	sql = statement3.str().c_str();


	/*example of insert value sql statement
	sql = "INSERT INTO"\
		" RUN_DATA (SIM_ID,RUN_ID,TIME_STEP,VECH_ID,VECH_X,VECH_Y,VECH_Z,VECH_ANGLE,VECH_TYPE,VECH_SPEED,VECH_SLOPE) "  \
		"VALUES (1,1,0.02,1,0,0,0,90,'SUV',10,2); " \
		"INSERT INTO RUN_DATA (SIM_ID,RUN_ID,TIME_STEP,VECH_ID,VECH_X,VECH_Y,VECH_Z,VECH_ANGLE,VECH_TYPE,VECH_SPEED,VECH_SLOPE) "  \
		"VALUES (2,1,0.04,1,0,10,0,90,'SUV',10 ,2); "     \
		"INSERT INTO RUN_DATA (SIM_ID,RUN_ID,TIME_STEP,VECH_ID,VECH_X,VECH_Y,VECH_Z,VECH_ANGLE,VECH_TYPE,VECH_SPEED,VECH_SLOPE)" \
		"VALUES (3,1,0.06,1,0,20,0,0,'SUV',10,2);" \
		"INSERT INTO RUN_DATA (SIM_ID,RUN_ID,TIME_STEP,VECH_ID,VECH_X,VECH_Y,VECH_Z,VECH_ANGLE,VECH_TYPE,VECH_SPEED,VECH_SLOPE)" \
		"VALUES (4,1,0.08,1,0,30,0,0,'SUV',10,2);";*/

		/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Records created successfully\n");
	}
}

void EventTree::showdata(dataMap *tablemap)
{
	/* Create SQL statement */

	string statement1;
	stringstream statement2;
	for (auto mapIterator = tablemap->begin(); mapIterator != tablemap->end(); ++mapIterator)
	{
		statement1 += (mapIterator->first)+" ";
	}
	/*print show data sql statement example : SELECT * FROM TABLE_NAME */
	statement2 << "SELECT * from " << statement1;
	sql = statement2.str().c_str();

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void*)datas, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "select Operation done successfully\n");
	}
}

//void database::updatedata()
//{
//	/* Create merged SQL statement */
//	sql = "UPDATE RUN_DATA set RUN_ID = 2 where SIM_ID=2; " \
//		"SELECT * from RUN_DATA";
//
//	/* Execute SQL statement */
//	rc = sqlite3_exec(db, sql, callback, (void*)datas, &zErrMsg);
//
//	if (rc != SQLITE_OK) {
//		fprintf(stderr, "SQL error: %s\n", zErrMsg);
//		sqlite3_free(zErrMsg);
//	}
//	else {
//		fprintf(stdout, "update Operation done successfully\n");
//	}
//}
//
//void database::deletedata()
//{
//	/* Create merged SQL statement */
//	sql = "DELETE from RUN_DATA where SIM_ID=2; " \
//		"SELECT * from RUN_DATA";
//
//	/* Execute SQL statement */
//	rc = sqlite3_exec(db, sql, callback, (void*)datas, &zErrMsg);
//
//	if (rc != SQLITE_OK) {
//		fprintf(stderr, "SQL error: %s\n", zErrMsg);
//		sqlite3_free(zErrMsg);
//	}
//	else {
//		fprintf(stdout, "delate Operation done successfully\n");
//	}
//}