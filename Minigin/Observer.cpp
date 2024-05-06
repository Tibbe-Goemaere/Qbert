#include "Observer.h"

void dae::Subject::AddObserver(Observer* pObserver)
{
	m_pObservers.push_back(pObserver);
}

void dae::Subject::RemoveObserver(Observer* pObserver)
{
	m_pObservers.erase(std::remove(m_pObservers.begin(),m_pObservers.end(),pObserver));
}

void dae::Subject::NotifyObservers(Event event, GameObject* pObject)
{
	for (const auto& observer : m_pObservers)
	{
		observer->Notify(event, pObject);
	}
}
