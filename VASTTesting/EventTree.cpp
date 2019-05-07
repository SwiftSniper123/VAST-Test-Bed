#include "VC_HEADERS.h"

// public functions---------------------------------------//

EventTree::EventTree(double timeSlice, ratio timeRatio, double endTime, string databaseName)
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

	_runID = 0;
	_scenario_ID = 0;
	_timeSlice = timeSlice;
	_timeRatio = timeRatio;
	_endTime = endTime;
	_simClock = -1;
	_numRuns = 1;
	_future = nullptr;
	_metrics = new tableMap();
	_componentInitialStateMap = new tableMap();
	_componentPresentStateMap = new tableMap();
	opendatabase(databaseName);
}

EventTree::EventTree(double timeSlice, ratio timeRatio, double endTime, int numRuns, string databaseName)
{
	if (timeSlice <= 0.0)
	{
		throw InvalidArgumentException("Time slices must be greater than 0 seconds.");
	}
	else if (timeRatio <= 0.0 || timeRatio > 1.0)
	{
		throw InvalidArgumentException("Time ratio must be greater than 0.");
	}
	else if (endTime <= 0.0)
	{
		throw InvalidArgumentException("Run end time must be greater than 0 seconds.");
	}
	else if (numRuns < 1)
	{
		throw InvalidArgumentException("Must have at least one run to test the scenario.");
	}

	_runID = 0;
	_scenario_ID = 0;
	_timeSlice = timeSlice;
	_timeRatio = timeRatio;
	_endTime = endTime;
	_simClock = -1;
	_numRuns = numRuns;
	_future = nullptr;
	_metrics = new tableMap();
	_componentInitialStateMap = new tableMap();
	_componentPresentStateMap = new tableMap();
	opendatabase(databaseName);
}

EventTree::EventTree(double timeSlice, ratio timeRatio, double endTime, int numRuns, string databaseName, int scenarioID)
{
	if (timeSlice <= 0.0)
	{
		throw InvalidArgumentException("Time slices must be greater than 0 seconds.");
	}
	else if (timeRatio <= 0.0 || timeRatio > 1.0)
	{
		throw InvalidArgumentException("Time ratio must be greater than 0.");
	}
	else if (endTime <= 0.0)
	{
		throw InvalidArgumentException("Run end time must be greater than 0 seconds.");
	}
	else if (numRuns < 1)
	{
		throw InvalidArgumentException("Must have at least one run to test the scenario.");
	}

	_runID = 0;
	_scenario_ID = scenarioID;
	_timeSlice = timeSlice;
	_timeRatio = timeRatio;
	_endTime = endTime;
	_simClock = -1;
	_numRuns = numRuns;
	_future = nullptr;
	_metrics = new tableMap();
	_componentInitialStateMap = new tableMap();
	_componentPresentStateMap = new tableMap();
	opendatabase(databaseName);
}

EventTree::~EventTree()
{
	// completely close down operations
	this->_runID = 0;
	closedatabase();

	// clear out data maps
	_metrics->clear();
	_componentInitialStateMap->clear();
	_componentPresentStateMap->clear();

	// clear out the linked list of Futures and the attached ComponentEventUpdates.
	resetFutureListOfEvents();
}

void EventTree::registerComponent(VComponent* vc)
{
	bool running = this->running();
	// only register components before a set of replications
	if (!running && vc->getVCType() != VComponent::VCType::ScenarioMetric)
	{
		tableMap::iterator it;
		it = _componentInitialStateMap->find(vc);
		if (it == _componentInitialStateMap->end())
		{
			// register this EventTree with the component so EventTree::addEvent 
			// can be called from within the component
			vc->registerEventTree(this);
			// store initial state (not modified after this)
			_componentInitialStateMap->emplace(vc, vc->getDataMap());
			// and initialize present state (modified)
			_componentPresentStateMap->emplace(vc, vc->getDataMap());

			// if lead component hasn't been set, give it a first-come first-serve vc
			_leadComponent = _leadComponent == nullptr ? vc : _leadComponent;
		}
	}
	else if (running)
	{
		closedatabase();
		throw OutOfTimeException(vc->getName(), string("register VComponent"), running);
	}
	else if (vc->getVCType() == VComponent::VCType::ScenarioMetric)
	{
		closedatabase();
		throw InvalidArgumentException("Cannot add a Scenario Metric. Use EventTree::registerMetric instead.");
	}
}

void EventTree::registerMetric(VComponent* comp)
{
	ScenarioMetric* sm;
	if (comp->getVCType() != VComponent::VCType::ScenarioMetric)
	{
		throw InvalidArgumentException(comp->getName() + " cannot be registered as a metric.");
	}
	else
	{
		sm = (ScenarioMetric*)comp;
		bool running = this->running();
		// only register components before a set of replications
		if (!running)
		{
			tableMap::iterator it;
			it = _metrics->find(sm);
			if (it == _metrics->end())
			{
				sm->registerEventTree(this);
				_metrics->emplace(sm, sm->getDataMap());
			}
		}
		else
		{
			closedatabase();
			throw OutOfTimeException(sm->getName(), string("register ScenarioMetric"), running);
		}
	}
	
}

int EventTree::getNumberOfVComp() const
{ return _componentInitialStateMap->size(); }

void EventTree::setFirstComponent(VComponent* vc)
{ _leadComponent = vc; }

VComponent* EventTree::getFirstComponent()
{	return _leadComponent;}

void EventTree::start()
{
	// initialize the tables for each component's run data
	createtable(_componentInitialStateMap, "Run_Data");
	//createtable(_metrics, "Run_Data");

	// these need to be composed sometime between parsing and starting emf4/28/2018
	//createtable(_VASTconfiguration, "Configuration");
	//createtable(_EnvironmentConfiguration, "Configuration");
	//createtable(_AVConfiguration, "Configuration"); 
	
	cout << "Starting " << _numRuns << " replications..." << endl;
	while (_numRuns > 0)
	{
		++_runID;
		replication();
	}
}

void EventTree::addEvent(VComponent* _eventSource, timestamp _eventTime, dataMap _eventDataMap)
{
	double currentSchedulableTime = _simClock < 0? 0: _simClock;
	
	// if this is a metric reporting, just update the data in the _metrics tablemap and then return
	if (_eventSource->getVCType() == VComponent::VCType::ScenarioMetric)
	{
		publishMetrics(_eventSource->getName(), _eventDataMap);
		//not currently used because metrics are not updated live
		//map<VComponent*, dataMap>::iterator metricMapIterator;
		//metricMapIterator = _metrics->find(_eventSource);
		//for (auto updateIterator = _eventDataMap.begin();
		//	updateIterator != _eventDataMap.end();
		//	++updateIterator)
		//{
		//	// get the present map's old component data, and the new update data, and overwrite
		//	VType* oldData = metricMapIterator->second[updateIterator->first];
		//	VType* newData = updateIterator->second;
		//	*metricMapIterator->second[updateIterator->first] = *newData;
		//}

		return;
	}
	// if an event is set outside of the run time:
	if(_eventTime < 0 || _eventTime < currentSchedulableTime - _timeSlice)  // too far left
	{
		closedatabase();
		throw OutOfTimeException(currentSchedulableTime, _eventSource->getName(), _eventTime);
	}
	// otherwise if this event happens in the distant future
	else if (_eventTime >= getEndSimTime())  // too far right
	{
		return; // leave the addEvent to finish the replication
	}
	// if the time clock has not yet started, everything is a Future
	else if (_simClock < 0)
	{
		findFuture(_eventSource, _eventTime, _eventDataMap, 0.0);
	}
	// otherwise sort this update into timeslices, starting with present 
	else if (_eventTime >= 0 && _eventTime < currentSchedulableTime + _timeSlice)  // "now"
	{
		// update the present map
		map<VComponent*, dataMap>::iterator presentMapIterator; // used to point to relevant VComponent & datamap
		if (stillAddingEvents(_eventSource->getName()))
		{
			presentMapIterator = _componentPresentStateMap->find(_eventSource); // try to find this VComponent data map
			// if we did find it
			if (presentMapIterator != _componentPresentStateMap->end())
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
					*presentMapIterator->second[updateIterator->first] = *newData;
				}
			}
			else
			{
				closedatabase();
				// we didn't find the VComponent in the state map, which is a problem
				throw InvalidArgumentException("Current system does not contain component: " + _eventSource->getName());
			}

			// now update the other components - CHIEF TASK
			VComponent* targetComponent;
			/*VComponent::VCType sourceVCType = _eventSource->getVCType();
			VComponent::VCType targetVCType = targetComponent->getVCType();*/

			for (presentMapIterator = _componentPresentStateMap->begin();
				presentMapIterator != _componentPresentStateMap->end();
				++presentMapIterator)
			{
				targetComponent = presentMapIterator->first;
				if (targetComponent != _eventSource)
				{
					targetComponent->update(_eventTime, _eventDataMap);
				}
			}
		}
	}
	// if this update takes place in the near future, check to see if there is a Future for it
	else if (_eventTime > 0 && _eventTime >= currentSchedulableTime + _timeSlice) // future(s)	
	{
		double targetTimeSlice = currentSchedulableTime + _timeSlice;
		findFuture(_eventSource, _eventTime, _eventDataMap, targetTimeSlice);
	} // done with futures
}

const double EventTree::getTimeRatio()
{ return _timeRatio; }

const double EventTree::getTimeSlice()
{ return _timeSlice; }

const timestamp EventTree::getCurrentSimTime()
{ return _simClock; }

const int EventTree::getNumberOfEarlyStops()
{ return _numEarlyStops; }

const timestamp EventTree::getEndSimTime()
{ return _endTime; }

bool EventTree::running() 
{ 
	bool result = false;
	if (_simClock >= 0 && /* Current time >= 0*/
		_simClock < _endTime && /* Less than end time */
		_endTime - _simClock > _timeSlice*_timeRatio/1000) /* Excluding small floating point differences and clock overshoot.*/
	{
		result = true;
	}
	return result; 
}
string EventTree::getRunID()
{
	stringstream ss;
	/* if the run ID is not zero, the scenario has passed initialization and can 
		write to the database about replication data.*/
	if (this->_runID > 0)
	{
		ss << this->_runID;
	}
	else
	{
		closedatabase();
		throw OutOfTimeException(string("EventTree"), string("get run replication ID from"), running());
	}
	return ss.str();
}

//-----------------------------------------------------------------------------//
// ----------------------Private functions ------------------------------------//
void EventTree::replication()
{
	timestamp originalEndTime = _endTime;
	if (_simClock == -1)
	{
		// TODO: add replication ID generator here		

		if (_leadComponent != nullptr)
		{
			// add an event in the future to initialize eventlist
			_leadComponent->update(0, dataMap());
		}

		_simClock = 0; 

		/* continue looping through the entire replication:
		1) check for events,
		2) update future list,
		3) publish to ScenarioMetrics,
		4) publish to database,
		5) check database for collision, 
		6) check to see if it's time to stop
		7) if not, increment time clock
		*/
		while (_simClock < _endTime)
		{
			// check for future events, advance the future list
			if (_future != nullptr)
			{
				Future* nextInLine = _future->nextFuture;

				// grab the stored events for execution at this timeslice
				ComponentEventUpdate* endOfList = _future->tailEvent;
				ComponentEventUpdate* sortedEvent = _future->headEvent;

				// cleanup the Future because it is now the present, set the new _future
				delete _future;
				_future = nextInLine;

				if (sortedEvent != nullptr)
				{
					// bubble sort to events by time
					int swaps;
					do
					{
						swaps = 0;
						while (sortedEvent->_next_ != nullptr)
						{
							ComponentEventUpdate* next = sortedEvent->_next_;
							if (sortedEvent->_time_ < sortedEvent->_next_->_time_)
							{
								//swap a<-S<->N->b; t
								ComponentEventUpdate* temp = sortedEvent->_prev_; // t = a
								next->_prev_ = temp; // a<-S->N->b; a<-N
								temp = next->_next_; // t = b
								sortedEvent->_next_ = temp; // a<-S->b; a<-N
								temp = sortedEvent; // t = S
								next->_next_ = sortedEvent; // a<-S->b; a<-N->S
								sortedEvent->_prev_ = next; // a<-N<->S->b  fully swapped
								swaps = 1;
							}
							sortedEvent = next; // move to next event
						}
						endOfList = sortedEvent;
					} while (swaps); // sorted is 0 when nothing had to be organized
				} 
				// list ends up with smallest at the end, and cursor at the end, so wind down	
				// execute stored events - unpackage them
				while(endOfList != nullptr)
				{
					VComponent* source = endOfList->_vc_;
					timestamp time = endOfList->_time_;
					dataMap data = endOfList->_data_;
					ComponentEventUpdate* deletable = endOfList;
					endOfList = endOfList->_prev_;
					delete deletable;
					addEvent(source, time, data);
				}
			}

			// perhaps an exchange with the ScenarioMetrics here?

			// publish to the database with the events for this timeslice, before it advances
			publishUpdates();

			// check for collisions, which would mean early stop
			listenForCollision();

			// check to make sure the replication isn't over early because of what we just published
			if (_endTime == _simClock) // time to stop
				break;
			// clear more room for events to be added by components for the next time slice
			reportedComponents.clear();
			advanceClock();
		}

		stop();
		// console reporting about the replication success or failure
		if (_endTime != originalEndTime)
		{
			cout << "Run " << _runID << " FAILED at " << _endTime << "." << endl;
		}
		else
		{
			cout << "Run " << _runID << " SUCCEEDED. " << endl;
		}
	}
}

bool EventTree::stillAddingEvents(string componentName)
{
	bool accepting = true;
	int alreadyReported = reportedComponents.size(); // how many have reported
	if (_leadComponent->getName().compare(componentName)!= 0) // ignore lead
	{
		reportedComponents.insert(componentName); // try adding this one
		accepting = alreadyReported != reportedComponents.size(); // did the size change?
	}	
	// we will not accept this event if the size is the same
	return accepting;
}

void EventTree::advanceClock()
{
	// wait for a time slice
	sleep_for(microseconds(int(_timeSlice*_timeRatio * 1000000)));

	// advance the simClock time by a time slice
	_simClock += getTimeSlice();  //***THIS IS THE ONLY PLACE THIS IS MODIFIED***
}

void EventTree::findFuture(VComponent* _eventSource, timestamp _eventTime, dataMap _eventDataMap, timestamp targetTimeSlice)
{
	//sort the incoming Event into timeslice Futures and subsequent Component Events
	Future* peek = _future;
	Future* last = nullptr;
	while (_eventTime >= targetTimeSlice)
	{
		if (peek == nullptr && last == nullptr)
		{
			//put new Future here
			peek = new Future(targetTimeSlice);
			_future = peek; // set first future in line
		}
		else if (peek == nullptr && last != nullptr)
		{
			peek = new Future(targetTimeSlice);
			last->nextFuture = peek;
		}
		// if this event is still further in the future, increment the target timeslice and loop again
		if (_eventTime >= targetTimeSlice + _timeSlice)
		{
			targetTimeSlice += _timeSlice;
			last = peek;
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
					// update the time only if it's the most up to date event
					if (storedEvents->_time_ >= _eventTime)
					{
						storedEvents->_time_ = _eventTime; // maintain the most current "raw", non-timesliced time
						// store each new data type
						dataMap::iterator storedEventData; // used to point to relevant data found in this stored event's data map
						for (auto updateDataIterator = _eventDataMap.begin();
							updateDataIterator != _eventDataMap.end();
							++updateDataIterator)
						{
							// write to the storedEvent data whether the key is found or not
							storedEvents->_data_.insert(namedData(updateDataIterator->first, updateDataIterator->second));
						}
					}
					else
					{
						// this event happened before the "latest" event, do not overwrite the time
						dataMap::iterator storedEventData; // used to point to relevant data found in this stored event's data map
						for (auto updateDataIterator = _eventDataMap.begin();
							updateDataIterator != _eventDataMap.end();
							++updateDataIterator)
						{
							storedEventData = storedEvents->_data_.find(updateDataIterator->first); // try to find this key in the map
							if (storedEventData == storedEvents->_data_.end())
							{
								// only write when the key is not found
								storedEvents->_data_.insert(namedData(updateDataIterator->first, updateDataIterator->second));
							}
						}
					}
					stored = true; // the event has been absorbed into the ComponentEventUpdate and will get executed later as a single addEvent
					break;
				}
				else
				{
					storedEvents = storedEvents->_next_;
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
					temp->_prev_ = peek->tailEvent;
					// update tail pointer to point to new construct
					peek->tailEvent = temp;
					break; // no longer need to loop through while (_eventTime >= targetTimeSlice)
				}
			} // looked through old and created new ComponentEventUpdate if necessary
		} // event has been stored
	} // end of while loop "while (_eventTime >= targetTimeSlice)"
}

void EventTree::earlyStop()
{
	_endTime = _simClock;
	_numEarlyStops++;
}

void EventTree::stop()
{
	if (!running())
	{	
		_simClock = -1;
		_numRuns--;
				
		// clear out Events and start with empty present Future
		resetFutureListOfEvents();

		// reset _componentPresentStateMap to _componentInitialStateMap
		for (auto presentMapIterator = _componentPresentStateMap->begin();
			presentMapIterator != _componentPresentStateMap->end();
			++presentMapIterator)
		{
			// resets the central map with initial values
			presentMapIterator->second = _componentInitialStateMap->at(presentMapIterator->first);

			// tell components to restart themselves
			presentMapIterator->first->stopReplication((_numRuns > 0), "" + this->_runID);
		}	
	}
} // end of stop()

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
