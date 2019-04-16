#include "EventTree.h"

// public functions---------------------------------------//

EventTree::EventTree(double timeSlice, ratio timeRatio, double endTime)
{
	if (timeSlice <= 0.0)
	{
		throw InvalidArgumentException("Time slices must be greater than 0 seconds.");
	}
	else if (timeRatio <= 0.0 || timeRatio > 1.0)
	{
		throw InvalidArgumentException("Time ratio must be greater than 0.");
	}
	else if(endTime <= 0.0)
	{
		throw InvalidArgumentException("Run end time must be greater than 0 seconds.");
	}

	_timeSlice = timeSlice;
	_timeRatio = timeRatio;
	_endTime = endTime;
	_simClock = -1;
	_clockThread = nullptr;
	_future = nullptr;
}

// -- unused at this time, but if extended would allow the user to send different units of time to the database
//EventTree::EventTree(double timeSlice, ratio timeRatio, double endTime, char databaseTimeUnit)
//{
//	if (timeSlice <= 0.0)
//	{
//		throw InvalidArgumentException("Time slices must be greater than 0 seconds.");
//	}
//	else if (timeRatio <= 0.0 || timeRatio > 1.0)
//	{
//		throw InvalidArgumentException("Time ratio must be greater than 0.");
//	}
//	else if (endTime <= 0.0)
//	{
//		throw InvalidArgumentException("Run end time must be greater than 0 seconds.");
//	}
//	switch (databaseTimeUnit)
//	{
//	case 's': _timeUnit = 's';
//		break;
//	case 'm': _timeUnit = 'm';
//		break;
//	case 'h': _timeUnit = 'h';
//		break;
//	case 'd': _timeUnit = 'd';
//		break;
//	case 'w': _timeUnit = 'w';
//		break;
//	default: throw InvalidArgumentException("The data base time unit must be s for seconds," \
//		" m for minutes, h for hours, d for days, or w for weeks.");
//	}
//
//	_timeSlice = timeSlice;
//	_timeRatio = timeRatio;
//	_endTime = endTime;
//	_simClock = -1;
//	_clockThread = nullptr;
//}

EventTree::~EventTree()
{
	unique_lock<mutex> lock(_clockLock);
	if(_clockThread != nullptr)
		_clockThread->join();
	// destroy linked list of Events
	resetFutureListOfEvents();
}

void EventTree::registerComponent(VComponent* vc)
{
	if (!running())
	{
		map<VComponent*, dataMap>::iterator it;

		it = _componentInitialStateMap.find(vc);
		if (it == _componentInitialStateMap.end())
		{
			_componentInitialStateMap.emplace(vc, vc->getDataMap());
			_componentPresentStateMap.emplace(vc, vc->getDataMap());
		}

		// TODO: add intialization of database tables here
		//  get vc type "AV_Avatar" etc, insert entry to that table
		//  create a table for this "AV" and insert initial values
	}
}

int EventTree::getNumberOfVComp() const
{ 
	return _componentInitialStateMap.size();
}

/* Starts the clock by advancing to relative time 0.  Records the wall clock start time.*/
void EventTree::start()
{
	unique_lock<mutex> lock(_clockLock);
	if (_simClock == -1)
	{
		// TODO: add run ID generator here		
		_runID = setRunID();
		_simClock = 0; // does not matter if data maps are all initialized before clock is advanced
		lock.unlock();

		 _clockThread = new thread ([&](EventTree* et)
		 {
			et->advanceClock(		
				std::ref(_timeSlice),
				std::ref(_timeRatio),
				std::ref(_endTime));
		 }, this);	
		
		 // create a thread whose sole job is to wait for the time to stop
		 thread stopThread([&](EventTree* et)
		 {
			 et->stop();
		 }, this);

	}
	else
	{
		lock.unlock();
	}
	
}

void EventTree::earlyStop()
{
	unique_lock<mutex> lock(_clockLock);
	_endTime = _simClock;
	stop();
}

void EventTree::stop()
{
	while (1)
	{
		if (!running())
		{

			_clockThread->join();
			_clockThread = nullptr;
			// _runID will persist until another start is called
			// _timeSlice, _timeRatio, _endTime, and _timeUnit persist from run to run

			// TODO: store the actual _simClock's last time somewhere
			unique_lock<mutex> lock(_clockLock);
			_simClock = -1;
			lock.unlock();

			// clear out Events and start with empty present Future
			resetFutureListOfEvents();

			// reset _componentPresentStateMap to _componentInitialStateMap
			lock.lock();
			_componentPresentStateMap.clear();
			_componentPresentStateMap = _componentInitialStateMap;
			lock.unlock();
		}
		
	}
}

/* addEvent - Adds a data update to the EventTree; sorts where the event is relevant and checks to
	see if it is time to advance the clock.  AddEvent throttles the incoming events so that they are 
	only published if it is their timeslice to do so.  
	_eventSource		The component that originates the update event
	_eventTime			The simulation timestamp in which the event occurred
	_eventDataMap		The map containing the new information.  If the data in the map
						is not different from its previous recorded value, this will
						throw an exception.*/
void EventTree::addEvent(VComponent* _eventSource, timestamp _eventTime, dataMap _eventDataMap)
{
	// clock is 0 or greater, and less than endTime, i.e. within runtime; other events are ignored
	if (running()) 
	{
		// lockdown the whole event, because we assume: we have not reached endTime, and the _simClock is not changing
		unique_lock<mutex> lock(_clockLock);

		// slow times should throw exception because we've already sent that data, 
		// the event took place more than one timeslice ago, that component is running 
		// too slow
		if(_eventTime < getCurrentSimTime() - getTimeSlice())  // too far left
		{
			throw OutOfTimeException(getCurrentSimTime(), _eventSource->getName(), _eventTime);
		}
		// otherwise if this event happens in the distant future
		else if (_eventTime >= getEndSimTime())  // too far right
		{
			throw OutOfTimeException(getCurrentSimTime(), _eventSource->getName(), _eventTime);
		}
		// otherwise sort this update into the Events, starting with present Future
		else if (_eventTime < getCurrentSimTime() + getTimeSlice())  // "now"
		{
			// update the present map
			map<VComponent*, dataMap>::iterator presentMapIterator; // used to point to relevant VComponent & datamap
			presentMapIterator = _componentPresentStateMap.find(_eventSource); // try to find this VComponent data map
			// if we did find it
			if (presentMapIterator != _componentPresentStateMap.end())
			{
				// then for every value in present _eventDataMap, overwrite the 
				// corresponding element in the componentPresentState state map
				for (auto updateIterator = _eventDataMap.begin();
					updateIterator != _eventDataMap.end();
					++updateIterator)
				{
					// get the present map's old component data, and the new update data, and overwrite
					VType* oldData = presentMapIterator->second[updateIterator->first];
					VType* newData = updateIterator->second;
					oldData = newData;
				}
			}
			else
			{
				// we didn't find the VComponent in the state map, which is a problem
				throw InvalidArgumentException("Current tracked data does not contain a record of " + _eventSource->getName());
			}

			// update the other components - CHIEF TASK
			for (presentMapIterator = _componentPresentStateMap.begin();
				presentMapIterator != _componentPresentStateMap.end();
				++presentMapIterator)
			{
				VComponent* targetComponent = presentMapIterator->first;
				if (targetComponent != _eventSource)
				{
					targetComponent->update(_eventTime, _eventDataMap);
				}
			}
		}
		// if this update takes place in the near future, check to see if there is a Future for it
		else if (_eventTime >= getCurrentSimTime() + getTimeSlice()) // future(s)	
		{
			//then sort the incoming Event into timeslice Futures and subsequent Component Events
			//where target time < thisTimeSlice
			double targetTimeSlice = getCurrentSimTime() + getTimeSlice();
			Future* peek = _future;
			while (_eventTime >= targetTimeSlice)
			{
				if (peek == nullptr)
				{
					//put new Future here
					peek = new Future(targetTimeSlice);
				}
				// if this event is still further in the future, increment the target timeslice and loop again
				if (_eventTime >= targetTimeSlice + getTimeSlice())
				{
					targetTimeSlice += getTimeSlice();
					peek = peek->nextFuture;
					continue;
				}
				// otherwise we have found the right time frame for this event
				else
				{
					//look for ComponentEventUpdate to store this in
					bool stored = false;
					ComponentEventUpdate* storedEvents = peek->headEvent;
					while (storedEvents != nullptr)
					{
						if (storedEvents->_vc_ == _eventSource) // then store it
						{
							// update the time
							storedEvents->_time_ = _eventTime; // maintain the most current "raw", non-timesliced time
							// store each new data type
							dataMap::iterator storedEventData; // used to point to relevant data found in this stored event's data map
							for(auto updateDataIterator = _eventDataMap.begin();
									updateDataIterator != _eventDataMap.end();
									++updateDataIterator)
							{ 
								storedEventData = storedEvents->_data_.find(updateDataIterator->first); // try to find this key in the map
								if (storedEventData != storedEvents->_data_.end())
								{
									// update the found VType
									storedEvents->_data_[updateDataIterator->first] = updateDataIterator->second;
								}
								else // if the matching data VType is not found by key
								{
									storedEvents->_data_.emplace(updateDataIterator->first, updateDataIterator->second);
								}
								// loop through remaining updated data
							}
							stored = true; // the event has been absorbed into the ComponentEventUpdate and will get executed later as a single addEvent
							break;
						}
					}
					if (!stored) // then store it
					{
						// were there any events inside the Future construct?
						if (peek->headEvent == nullptr)
						{
							peek->headEvent = new ComponentEventUpdate(_eventSource, _eventTime, _eventDataMap);
							peek->tailEvent = peek->headEvent;
							break; // no longer need to loop through while (_eventTime >= targetTimeSlice)
						}
						else
						{
							// new construct
							ComponentEventUpdate* temp = new ComponentEventUpdate(_eventSource, _eventTime, _eventDataMap);
							// link old construct to new construct
							peek->tailEvent->_next_ = temp;
							// update tail pointer to point to new construct
							peek->tailEvent = temp;
							break; // no longer need to loop through while (_eventTime >= targetTimeSlice)
						}						
					} // looked through old and created new ComponentEventUpdate if necessary
				} // event has been stored
			} // end of while loop "while (_eventTime >= targetTimeSlice)"
		} // done with futures
		lock.unlock();
	} // end of condition check for running()	
}

double EventTree::getTimeRatio()
{ return _timeRatio; }

double EventTree::getTimeSlice()
{ return _timeSlice; }

timestamp EventTree::getCurrentSimTime()
{ 
	unique_lock<mutex> lock(_clockLock);
	return _simClock; 
}

timestamp EventTree::getEndSimTime()
{ 
	unique_lock<mutex> lock(_clockLock);
	return _endTime; 
}

bool EventTree::running() 
{ 
	unique_lock<mutex> lock(_clockLock);
	bool result = false;
	if (_simClock >= 0 && /* Current time >= 0*/
		_simClock < _endTime && /* Less than end time */
		_endTime - _simClock > _timeSlice*_timeRatio/1000) /* Excluding small floating point differences and clock overshoot.*/
	{
		//lock.unlock();
		result = true;
	}
	return result; 
}

string EventTree::setRunID() 
{
	// TODO: generate run ID
	return "runID generator function needs to be implemented";
}

// Private functions ------------------------------------//

void EventTree::publishUpdates()
{
	// send updates to database
}

void EventTree::advanceClock(double& __timeSlice, double& __timeRatio, double& __endTime)
{
	//unique_lock<mutex> lock(_clockLock);
	//double thisEndTime = __endTime; // the original endTime may change elsewhere
	//double thisSimClockTime = __simClock;
	//lock.unlock();

	// begin the march of the clock timeslice by timeslice
	unique_lock<mutex> lock(_clockLock);
	while (_simClock < _endTime)
	{
		lock.unlock();
		// wait for a time slice
		sleep_for(microseconds(int(getTimeSlice()*getTimeRatio() * 1000000)));
		
		// publish to the database with the events for this timeslice, before it advances
		publishUpdates(); 

		// check for collisions, which would mean early stop

/////////////// lock ///////////////////
		// check to make sure the run isn't over early because of what we just published
		lock.lock();
		if (_endTime == _simClock) // time to stop
			break;
		
		// advance the simClock time by a time slice
		_simClock += getTimeSlice();  //***THIS IS THE ONLY PLACE THIS IS MODIFIED***
		//thisSimClockTime = _simClock;
		
		// advance the future list
		if (_future != nullptr)
		{
			Future* nextInLine = _future->nextFuture;

			// grab the stored events for execution at this timeslice
			ComponentEventUpdate* storedEvents = _future->headEvent;
			
			// cleanup the Future because it is now the present, set the new _future
			delete _future;
			_future = nextInLine;
			lock.unlock();
/////////////// unlock ///////////////////

			// now execute stored events - unpackage them
			while (storedEvents != nullptr)
			{
				thread storedAddEvent([&](EventTree* et)
				{
					et->addEvent(
						std::ref(storedEvents->_vc_),
						std::ref(storedEvents->_time_),
						std::ref(storedEvents->_data_));
				}, this);
				storedAddEvent.join(); // wait for that thread to end
				ComponentEventUpdate* deletable = storedEvents;
				storedEvents = storedEvents->_next_;
				delete deletable;
			}
		}
		else
		{
			lock.unlock();
		}
		lock.lock();
	}
	lock.unlock();
}

void EventTree::resetFutureListOfEvents()
{
	Future* node = _future;
	Future* deletableF = nullptr;
	while (node != nullptr)
	{
		ComponentEventUpdate* ceu = node->headEvent;
		ComponentEventUpdate* deletableC = nullptr;
		while (ceu != nullptr)
		{
			// move current node into position to be deleted
			deletableC = ceu;
			// store future node; may be nullptr
			ceu = deletableC->_next_;  // a.k.a. ceu->_next_, or next ComponentEventUpdate node
			delete deletableC;
		}
		// now that ComponentEventUpdates are cleared out of this Future
		// move current node into position to be deleted
		deletableF = node;
		// store future node; may be nullptr
		node = deletableF->nextFuture; // a.k.a. node->nextFuture, or next Future node
		delete deletableF;
	}
}