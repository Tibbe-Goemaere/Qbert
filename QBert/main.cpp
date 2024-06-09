#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <SDL.h>

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "FpsComponent.h"
#include "RotatingComponent.h"
#include "InputManager.h"
#include "HealthDisplayComponent.h"
#include "ScoreComponent.h"
#include "ScoreDisplayComponent.h"
#include "LevelComponent.h"
#include "QbertComponent.h"
#include "CostumCommands.h"
#include "SoundSystem.h"
#include "CoilyComponent.h"
#include "DiskComponent.h"
#include "SlickComponent.h"
#include "EnemySpawner.h"
#include "UpdateManager.h"
#include "UggComponent.h"

#include "GameManager.h"

using namespace dae;

void load(glm::vec2 windowSize)
{
	SoundLocater::RegisterSoundSystem(std::make_unique<SdlSoundSystem>());

	auto& gameManager = GameManager::GetInstance();
	gameManager.SetWindowSize(windowSize);
	gameManager.MakeMenu();
}

int main(int, char* [])
{
	dae::Minigin engine("../Data/");
	auto windowSize = engine.GetWindowSize();
	engine.Run([&]() { load(windowSize); });
	return 0;
}