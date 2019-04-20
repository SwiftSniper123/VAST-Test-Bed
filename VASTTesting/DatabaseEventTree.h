/* VAST 
Author: emf
*/
#ifndef EventTree_h
#define EventTree_h
#include "VComponent.h"
#include "sqlite3.h"

using std::string;
using std::map;

typedef double time;

class EventTree
{
private:
	void advanceClock()
	{};
	
	

	/*database callback function*/
	static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
		int i;
		for (i = 0; i < argc; i++) {
			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");
		return 0;
	}
public:
	/* Function called by external components to provide Event changes to the EventTree.  
	source	- the VAST component sending the event
	time	- the time the event is posted
	dataMap	- the set of data effected in the event*/
	void addEvent(VComponent* source, time time, map<string, VType> dataMap)
	{
	};

	time getScenarioTime()
	{
		return time(0.0);
	};

	void registerComponent(VComponent* component)
	{

	};
	
	void opendatabase();
	void EventTree::createtable(dataMap *tablemap, dataMap *avmap);
	void publishEvent(VComponent* source, time time, dataMap *tablemap, dataMap *avmap);

	/*show the data in the database*/
	void showdata(dataMap *tablemap);

	void updateDatabase(time time, dataMap data) {};

	/*database parameter*/
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char *sql;
	const char* datas = "Callback function called";

	


};

#endif