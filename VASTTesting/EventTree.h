#pragma once
#include <chrono> //C++ 14 required; sleep_for()
#include <thread>
#include <set>
#include "VComponent.h" // timestamp, dataMap
#include "sqlite3.h"
#include <exception>

using namespace std::this_thread;     // sleep_for, sleep_until
using std::chrono::milliseconds;
using std::chrono::microseconds;
typedef std::invalid_argument InvalidArgumentException;
typedef double ratio;
typedef map<VComponent*, dataMap> tableMap;

class EventTree
{
private:
	/* Exception for time clock discrepencies.*/
	class OutOfTimeException : public exception
	{
	public:
		/* Shows the error of adding an event out of sync with the sim clock.*/
		OutOfTimeException(timestamp currentTime, string offendingComponent, double offendingTime) :
			exception()
		{
			stringstream ss;
			ss << offendingComponent << " attempted to add an event at " << offendingTime
				<< " but the clock was at " << currentTime;
			_messageEnd = ss.str();
		}
		
		/* Shows the error of adding a VAST component when in the middle of replications.*/
		OutOfTimeException(string functionName, string offendingComponent, bool running)
		{
			stringstream ss;
			ss << "User cannot " << functionName << " " << offendingComponent
				<< " during this phase: ";
			if (running)
			{
				ss << "Replication Running";
			}
			else
			{
				ss << "No Replication Running";
			}
			_messageEnd = ss.str();
		}

		/* Overridden from parent std::exception.  Identifies this as an exception thrown
		by VAST for reasons specific to this system and the clock phases by which the system 
		operates.*/
		virtual const char* what() const throw ()
		{
			stringstream ss;
			ss << "VAST EventTree cannot perform this action at this clock phase: "
				<< _messageEnd;
			return ss.str().c_str();
		}
	private:
		string _messageEnd;
	};

	/* Exception for database discrepencies*/
	class DatabaseException : public exception
	{
	public:
		DatabaseException(string message) : exception()
		{
			_msg = message;
		}

		virtual const char* what() const throw ()
		{
			string result = "VAST Database Exception: " + _msg;
			return result.c_str();
		}

	private:
		string _msg;
	};

	/* ID for this replication.*/
	int _runID;

	/* Scenario id for all replications running.*/
	int _scenario_ID;

	/* The length of each timeslice that the clock progresses.*/
	double _timeSlice;

	/* Always number of simulated seconds per real seconds.  Can represent
	simulation speeds faster than real time or slower than real time.*/
	ratio _timeRatio;

	/* End time when the replication will end.*/
	double _endTime;

	/* Number of runs to perform.*/
	int _numRuns;

	/* Number of replications that stopped early.*/
	int _numEarlyStops = 0;

	/* The current time in the simulated world.*/
	double _simClock = -1;

	/* First component to be updated in the system, and therefore first 
	component to add an event.  "Drives" the other components to respond 
	by adding their own events.*/
	VComponent* _leadComponent = nullptr;

	std::set<string> reportedComponents;

	/* Registered metrics.*/
	tableMap* _metrics;

	/* The map of "past" values*/
	tableMap* _componentInitialStateMap;

	/* The map of "past" values*/
	tableMap* _componentPresentStateMap;

	/* Structure for storing incoming Events for later execution.*/
	struct ComponentEventUpdate
	{
		ComponentEventUpdate(VComponent* vc, double time, dataMap data)
		{
			_vc_ = vc;
			_time_ = time;
			_data_ = data;
		};
		VComponent* _vc_;
		double _time_;
		dataMap _data_;
		ComponentEventUpdate* _next_ = nullptr;
		ComponentEventUpdate* _prev_ = nullptr;
	};

	/* Structure for storing present andd future updates.*/
	struct Future
	{		
		Future(double time)
		{
			thisTimeSlice = time;
			//changedData = update;
		}

		double thisTimeSlice;
		ComponentEventUpdate* headEvent = nullptr;
		ComponentEventUpdate* tailEvent = nullptr;
		Future* nextFuture = nullptr;
	};

	/* The state of replication data as collected for the next timeslice.*/
	Future* _future;

	/* Begins the clock at 0, updates the current map for every event added.*/
	void replication();

	/* Tracks which components have replied to the lead component.*/
	bool stillAddingEvents(string componentName);

	/* Once the simClock moves into the next timeSlice, the old timeSlice's events will be discarded.*/
	void advanceClock();

	/* Generates a future timeslice record for the Event that cannot yet be executed.*/
	void findFuture(VComponent* _eventSource, timestamp _eventTime, dataMap _eventDataMap, timestamp targetTimeSlice);

	/* Stops the system ahead of the endTime*/
	void earlyStop();

	/* Immediately stops the simClock and refreshes the */
	void stop();

	/* Empties the Future linked list completely.*/
	void resetFutureListOfEvents();

	/*database parameters---------------------------------------*/
	sqlite3 *db = nullptr;
	
	/* Callback variable that stores error messages from the 
	SQLite3 library.*/
	char *zErrMsg = 0;

	/* Integer result of calls to sqlite3 library.  Zero is 
	SQLITE_OK and signifies no errors or exceptions.*/
	int rc;
	
	/* Creates a database file and opens the connection to it.  If a fileName is not provided, the default name is "VASTdatabase.db".*/
	void opendatabase(string fileName);

	/* Closes the database connection.  The file is preserved.*/
	void closedatabase();

	/* Creates all of the tables in the database to keep until the database is closed.*/
	void createtable(tableMap* componentsToData, const char* tableType);
		
	/* Publishes the configuration of this scenario.*/
	void publishConfigTable(string tableName, tableMap configurations);
	
	/* Publishes the update event to the database to all tables relevant to
	that component.  */
	void publishUpdates();

	/* Publishes the metric calculations at the end of the run.*/
	void publishMetrics(string name, dataMap publishMap);

	/*show the data in the database*/
	void listenForCollision();

	/*database callback function checking collision table*/
	static int collisionSearch(void* eventTreePointer, int argc, char **argv, char **azColName);

	/*empty callback function to do nothing*/
	static int callback(void* eventTreePointer, int argc, char **argv, char **azColName)
	{return 0;};

public:
	/* Creates an EventTree, sets the simClock to -1.  Random scenario ID.  Opens the database connection.
	timeSlice	The unit of timestamp by which update events are organized 
				and collected.  Throws an exception for values less than
				or equal to 0.
	timeRatio	The number of simulated seconds per real seconds.  Throws
				an exception for values less than or equal to 0.
	endTime		The end time for the replication.  Throws an exception for values less 
				than or equal to 0.
	Default number of runs is 1.*/
	EventTree(double timeSlice, ratio timeRatio, double endTime, string databaseName) ;

	/* Creates an EventTree, sets the simClock to -1.  Random scenario ID.  Opens the database connection.
	timeSlice			The unit of time by which update events are organized 
						and collected.  Throws an exception for values less than
						or equal to 0.
	timeRatio			The number of simulated seconds per real seconds.  Throws
						an exception for values less than or equal to 0.
	endTime				The end time for the replication.  Throws an exception for values less 
						than or equal to 0.
	numberOfRuns		Number of runs to be performed under these scenario parameters.
	databaseName		Desired file name of the database that will be generated.*/
	EventTree(double timeSlice, ratio timeRatio, double endTime, int numRuns, string databaseName);

	/* Creates an EventTree, sets the simClock to -1.  Random scenario ID.  Opens the database connection.
	timeSlice			The unit of time by which update events are organized
						and collected.  Throws an exception for values less than
						or equal to 0.
	timeRatio			The number of simulated seconds per real seconds.  Throws
						an exception for values less than or equal to 0.
	endTime				The end time for the replication.  Throws an exception for values less
						than or equal to 0.
	numberOfRuns		Number of runs to be performed under these scenario parameters.
	databaseName		Desired file name of the database that will be generated.
	scenarioID			The integer ID number given to this set of replications to identify the 
						configuration of parameters.*/
	EventTree(double timeSlice, ratio timeRatio, double endTime, int numRuns, string databaseName, int scenarioID);

	/* Destructor - destroys internal components, closes connection to database.*/
	~EventTree();

	/* A VComponent should register themselves with the EventTree and initialize the dataMap.*/
	void registerComponent(VComponent* vc);

	/* A Scenario Metric should register themselves with the EventTree.*/
	void registerMetric(VComponent* sm);

	/* Reports back the number of components that have been registered in the EventTree.*/
	int getNumberOfVComp()const;

	/* Sets which component is the first to receive updates.*/
	void setFirstComponent(VComponent* vc);

	/* Starts the clock by advancing to relative time 0.*/
	void start();

	/* Adds a data update to the EventTree; sorts where the event is relevant and checks to 
	see if it is time to advance the clock.
	source		The component that originates the update event
	time		The simulation timestamp in which the event occurred
	dataMap		The map containing the new information.  If the data in the map
				is not different from its previous recorded value, this will 
				throw an exception.*/
	void addEvent(VComponent* source, timestamp time, dataMap dataMap);

	/* Provides the clock speed ratio of simulated seconds per real 
	seconds at which this scenario replication progresses. */
	const double getTimeRatio();
		
	/* Provides the timeSlice for this scenario replication.*/
	const double getTimeSlice();

	/* Returns the current simClock time.  Returns -1 if the clock has not started.*/
	const timestamp getCurrentSimTime();

	/* Returns the number of early stopped replications.*/
	const int getNumberOfEarlyStops();

	/* Returns the future end timestamp of the simulation.*/
	const timestamp getEndSimTime();

	/* Starts the sim clock.  If the sim clock is starting or already started, it returns true.  If the end
	time has been reached, it returns false.*/
	bool running();

	/* Gets the replication ID for this replication.*/
	string getRunID();
};
