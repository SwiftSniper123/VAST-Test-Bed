#pragma once

class Event
{
	double time;
	//action
};

class EventTree : VComponent
{
public:
	void addEvent(Event event);
	void removeEvent();
private:
	Event *eventList;
};
