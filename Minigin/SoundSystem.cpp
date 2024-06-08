#include "SoundSystem.h"
#include "SDL_mixer.h"
#include <vector>
#include <stdexcept>
#include "mutex"
#include <queue>

namespace dae
{
	std::unique_ptr<dae::SoundSystem> SoundLocater::m_soundSystemInstance = std::make_unique<NullSoundSystem>();

	SoundSystem& dae::SoundLocater::GetSoundsystem()
	{
		return *m_soundSystemInstance;
	}

	void dae::SoundLocater::RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss)
	{
		if (ss == nullptr)
		{
			m_soundSystemInstance = std::make_unique<NullSoundSystem>();
		}
		else
		{
			m_soundSystemInstance = std::move(ss);
		}
	}


	class SdlSoundSystem::SdlSoundSystemImpl final : public SoundSystem
	{
	public:
		SdlSoundSystemImpl()
			:m_pAudioClips{}
			, m_filenames{}
			, m_soundsQueue{}
			, m_stopLoop{ false }
		{

			m_thread = std::jthread{ &SdlSoundSystem::SdlSoundSystemImpl::ProcessSoundQueue,this };
		}

		virtual ~SdlSoundSystemImpl()
		{
			m_stopLoop = true;
			m_shouldProccesSoundQueue.notify_one();

			if (m_thread.joinable()) {
				m_thread.join();
			}

			for (auto chunk : m_pAudioClips)
			{
				Mix_FreeChunk(chunk);
			}
			m_pAudioClips.clear();
			m_filenames.clear();
		}

		void Play(const std::string fn, const float)
		{
			Mix_Chunk* pAudioClip = nullptr;
			int idx = 0;

			if (m_filenames.size() <= 0)
			{
				pAudioClip = LoadAudioClip(fn);
			}
			else
			{
				auto it = std::find(m_filenames.begin(), m_filenames.end(), fn);

				if (it == m_filenames.end())
				{
					idx = static_cast<int>(m_filenames.size());
					pAudioClip = LoadAudioClip(fn);
				}
				else
				{
					idx = static_cast<int>(std::distance(m_filenames.begin(), it));
					pAudioClip = m_pAudioClips[idx];
				}

			}

			if (!(pAudioClip == nullptr))
			{
				m_soundsQueue.emplace(idx);
				m_shouldProccesSoundQueue.notify_one();
			}
		}

	private:
		Mix_Chunk* LoadAudioClip(const std::string fn)
		{
			std::lock_guard<std::mutex> lock(m_loadMutex);
			auto pAudioClip = Mix_LoadWAV(fn.c_str());
			m_pAudioClips.push_back(pAudioClip);
			m_filenames.push_back(fn);
			return pAudioClip;
		}
		
		void ProcessSoundQueue()
		{
			while (!m_stopLoop)
			{
				std::unique_lock<std::mutex> lock(m_queueMutex);
				m_shouldProccesSoundQueue.wait(lock, [&] {return !m_soundsQueue.empty() || m_stopLoop; });

				while (!m_soundsQueue.empty())
				{
					auto nextSoundIdx = m_soundsQueue.front();
					m_soundsQueue.pop();
					lock.unlock();
					Mix_PlayChannel(-1, m_pAudioClips[nextSoundIdx], 0);
					lock.lock();
				}
			}
		}

		std::vector<Mix_Chunk*> m_pAudioClips;
		std::vector<std::string> m_filenames;
		std::mutex m_loadMutex;
		std::mutex m_queueMutex;
		std::queue<int> m_soundsQueue;
		std::jthread m_thread;
		bool m_stopLoop;
		std::condition_variable m_shouldProccesSoundQueue;
	};

	dae::SdlSoundSystem::SdlSoundSystem()
	{
		m_pImpl = std::make_unique<SdlSoundSystemImpl>();
	}

	dae::SdlSoundSystem::~SdlSoundSystem() = default;

	void SdlSoundSystem::Play(const std::string fn, const float volume)
	{
		m_pImpl->Play(fn, volume);
	}
}

