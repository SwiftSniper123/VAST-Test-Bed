#pragma once
#include <chrono> //C++ 14 required; sleep_for()
#include <thread>
#include <mutex>
#include "VComponent.h" // timestamp, dataMap
#include <exception>


using namespace std::this_thread;     // sleep_for, sleep_until
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;
using std::thread;
using std::mutex;
using std::unique_lock;
typedef std::invalid_argument InvalidArgumentException;
typedef double ratio;

struct OutOfTimeException : public std::exception
{
	OutOfTimeException(timestamp currentTime, string offendingComponent, double offendingTime)
	{
		_currentTime = currentTime;
		_component = offendingComponent;
		_offendingTime = offendingTime;
	}
	const char* what() const throw ()
	{
		stringstream ss;
		ss << "VAST EventTree has received an event that was too slow for the system or beyond the run length: "
			<< _component << " attempted to add an event at " << _offendingTime 
			<< " but the clock was at " << _currentTime;
		return ss.str().c_str();
	}

	double _currentTime;
	string _component;
	double _offendingTime;
};

class EventTree
{
private:
	
	string _runID;

	/* The length of each timeslice that the clock progresses.*/
	double _timeSlice;

	/* Always number of simulated seconds per real seconds.  Can represent
	simulation speeds faster than real time or slower than real time.*/
	ratio _timeRatio;

	/* End time when the run will end.*/
	double _endTime;

	/* The current time in the simulated world.*/
	double _simClock = -1;

	///* Database time unit output.*/
	//char _timeUnit = 's';

	/* The map of "past" values*/
	map<VComponent*, dataMap> _componentInitialStateMap;

	/* The map of "past" values*/
	map<VComponent*, dataMap> _componentPresentStateMap;

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

	/* The state of run data as collected for the next timeslice.*/
	Future* _future;

	thread* _clockThread;

	mutex _clockLock;

	/* Empties the Future linked list completely.*/
	void resetFutureListOfEvents();

	/* Publishes the update event to the database to all tables relevant to
	that component.  */
	void publishUpdates();

	/* Once the simClock moves into the next timeSlice, the old timeSlice's events will be discarded.*/
	void advanceClock(double& __timeSlice, double& __timeRatio, double& __endTime);

public:
	/* Creates an EventTree, sets the simClock to -1.
	timeSlice	The unit of timestamp by which update events are organized 
				and collected.  Throws an exception for values less than
				or equal to 0.
	timeRatio	The number of simulated seconds per real seconds.  Throws
				an exception for values less than or equal to 0.
	endTime		The end time for the run.  Throws an exception for values less 
				than or equal to 0.
	Default time unit output to database is second.*/
	EventTree(double timeSlice, ratio timeRatio, double endTime) ;

	/* Creates an EventTree, sets the simClock to -1.
	timeSlice			The unit of time by which update events are organized 
						and collected.  Throws an exception for values less than
						or equal to 0.
	timeRatio			The number of simulated seconds per real seconds.  Throws
						an exception for values less than or equal to 0.
	endTime				The end time for the run.  Throws an exception for values less 
						than or equal to 0.
	databaseTimeUnit	Unit of time represented in the database: s, m, h, d, w
						for second, minute, hour, day, week.  Other entries will 
						throw an exception.*/
	EventTree(double timeSlice, ratio timeRatio, double endTime, char databaseTimeUnit);

	/* Destructor - destroys internal components*/
	~EventTree();

	/* A VComponent should register themselves with the EventTree and initialize the /.*/
	void registerComponent(VComponent* vc);

	/* Reports back the number of components that have been registered in the EventTree.*/
	int getNumberOfVComp()const;

	/* Starts the clock by advancing to relative time 0.  Records the wall clock start time.*/
	void start();

	/* Stops the system ahead of the endTime*/
	void earlyStop();

	/* Immediately stops the simClock and refreshes the */
	void stop();

	/* Adds a data update to the EventTree; sorts where the event is relevant and checks to 
	see if it is time to advance the clock.
	source		The component that originates the update event
	time		The simulation timestamp in which the event occurred
	dataMap		The map containing the new information.  If the data in the map
				is not different from its previous recorded value, this will 
				throw an exception.*/
	void addEvent(VComponent* source, timestamp time, dataMap dataMap);

	/* Provides the clock speed ratio of simulated seconds per real 
	seconds at which this scenario run progresses. */
	double getTimeRatio();
		
	/* Provides the timeSlice for this scenario run.*/
	double getTimeSlice();

	/* Returns the current simClock time.  Returns -1 if the clock has not started.*/
	timestamp getCurrentSimTime();

	///* Returns the running time of the simulation.*/
	//timestamp getStartSimTime() { return 0.0; };

	/* Returns the future end timestamp of the simulation.*/
	timestamp getEndSimTime() ;

	/* Starts the sim clock.  If the sim clock is starting or already started, it returns true.  If the end
	time has been reached, it returns false.*/
	bool running();

	/* Sets the run ID for this run.*/
	string setRunID();
};
