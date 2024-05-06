#pragma once
#include <memory>
#include <string>

namespace dae
{
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void Play(const std::string fn, const float volume) = 0;
	};
	
	class NullSoundSystem final : public SoundSystem
	{
		void Play(const std::string , const float ) override {}
	};

	class SoundLocater final
	{
		static std::unique_ptr<SoundSystem> m_soundSystemInstance;

	public:
		static SoundSystem& GetSoundsystem();
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss);	
	};

	class SdlSoundSystem final : public SoundSystem
	{
	public:
		SdlSoundSystem();
		SdlSoundSystem(SdlSoundSystem&& other) = delete;
		SdlSoundSystem(const SdlSoundSystem& other) = delete;
		SdlSoundSystem& operator=(SdlSoundSystem&& other) = delete;
		SdlSoundSystem& operator=(const SdlSoundSystem& other) = delete;
		virtual ~SdlSoundSystem();

		void Play(const std::string fn, const float volume) override;
	private:
		class SdlSoundSystemImpl;
		std::unique_ptr<SdlSoundSystemImpl> m_pImpl;
	};
}
