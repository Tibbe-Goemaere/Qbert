#pragma once
#include "Singleton.h"
#include <vector>
#include "ObjectUpdater.h"
#include <memory>

namespace dae
{
	class UpdateManager : public Singleton<UpdateManager>
	{
	private:
		std::vector<std::unique_ptr<ObjectUpdater>> m_pUpdaters;

	public:
		~UpdateManager() = default;

		void Update()
		{
			for (auto& pUpdater : m_pUpdaters)
			{
				pUpdater->Update();
			}
		}

		void AddUpdater(std::unique_ptr<ObjectUpdater> pUpdater)
		{
			m_pUpdaters.push_back(std::move(pUpdater));
		}

		void RemoveAllUpdaters()
		{
			m_pUpdaters.clear();
		}

		void Reset()
		{
			for (auto& pUpdater : m_pUpdaters)
			{
				pUpdater->Reset();
			}
		}
	
	};
}


