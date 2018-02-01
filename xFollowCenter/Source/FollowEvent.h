#ifndef FollowEvent_H
#define FollowEvent_H

#include <mutex>
#include <list>

class CFollowEvent
{
public:
	void pushEvent(int eventID, void* eventData = nullptr);
	std::pair<int, void*> getEvent();

private:
	struct stuEvent
	{
		int     eventID;
		void*   eventData;
	};
	std::mutex              m_mutex;
	std::list<stuEvent>     m_events;
};

inline void CFollowEvent::pushEvent( int eventID, void* eventData )
{
	stuEvent event;
	event.eventID = eventID;
	event.eventData = eventData;
	{
		std::lock_guard<std::mutex> lck(m_mutex);
		m_events.push_back(event);
	}
}

inline std::pair<int, void*> CFollowEvent::getEvent()
{
	int eventID = 0;
	void* eventData = nullptr;
	{
		std::lock_guard<std::mutex> lck(m_mutex);
		if (m_events.size() > 0)
		{
			eventID = m_events.begin()->eventID;
			eventData = m_events.begin()->eventData;
			m_events.pop_front();
		}
	}
	return std::make_pair(eventID, eventData);
}

#endif // FollowEvent_H
