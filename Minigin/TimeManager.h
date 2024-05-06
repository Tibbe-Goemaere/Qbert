#pragma once
#include "Singleton.h"

namespace dae
{
	class TimeManager final : public Singleton<TimeManager>
	{
	public:
		float GetDeltaTime() const { return m_deltaTime; };
		void SetDeltaTime(const float deltaTime) { m_deltaTime = deltaTime; }
	private:
		float m_deltaTime;
	};

}
