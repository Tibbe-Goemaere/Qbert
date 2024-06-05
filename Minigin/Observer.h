#pragma once
#include <vector>
namespace dae
{
	class GameObject;

	enum class Event
	{
		PlayerDies,
		ScoreChanged
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Event e, const GameObject* pObject) = 0;
	};

	class Subject
	{
	public:
		void AddObserver(Observer* pObserver);
		void RemoveObserver(Observer* pObserver);
		void NotifyObservers(Event event, GameObject* pObject);
	private:
		std::vector<Observer*> m_pObservers;
	};
}


