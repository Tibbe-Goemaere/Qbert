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
#include "MovePlayerComponent.h"
#include "CostumCommands.h"
#include "SoundSystem.h"
#include "MoveDownComponent.h"
#include "CoilyComponent.h"

using namespace dae;

void load()
{
	SoundLocater::RegisterSoundSystem(std::make_unique<SdlSoundSystem>());
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

	auto& scene = dae::SceneManager::GetInstance().CreateScene("Level1");

	auto level = std::make_unique<dae::GameObject>();
	level->AddComponent<dae::RenderComponent>();
	auto level1Component = level->AddComponent<dae::LevelComponent>("../Levels/Level1-0.xml");
	level->SetLocalPosition({ 288,50,0 });
	scene.Add(std::move(level));


	//Add UI
	auto uiPointsObject = std::make_unique<dae::GameObject>();
	uiPointsObject->AddComponent<dae::RenderComponent>();
	auto pointsText = uiPointsObject->AddComponent<dae::TextComponent>("Points", font);
	uiPointsObject->SetLocalPosition(glm::vec3(20, 160, 0));
	scene.Add(std::move(uiPointsObject));

	auto uiLivesObject = std::make_unique<dae::GameObject>();
	uiLivesObject->AddComponent<dae::RenderComponent>();
	auto livesText = uiLivesObject->AddComponent<dae::TextComponent>("LivesComp", font);
	uiLivesObject->SetLocalPosition(glm::vec3(20, 130, 0));
	scene.Add(std::move(uiLivesObject));

	//Qbert
	auto qbert = std::make_unique<dae::GameObject>();

	//RenderComponent
	auto renderQbert = qbert->AddComponent<dae::RenderComponent>();
	renderQbert->SetTexture("../Resources/Sprites/Qbert.png");

	//ScoreComponent
	qbert->AddComponent<dae::ScoreComponent>();
	qbert->AddComponent<dae::ScoreDisplayComponent>(pointsText);

	//Add Health Component + Score Component and displays
	qbert->AddComponent<dae::HealthComponent>();
	qbert->AddComponent<dae::HealthDisplayComponent>(livesText);

	//Movemement
	qbert->AddComponent<dae::MovePlayerComponent>(level1Component);

	auto pQbert = scene.Add(std::move(qbert));

	//Coily
	auto coily = std::make_unique<dae::GameObject>();

	//RenderComponent
	auto renderCoily = coily->AddComponent<dae::RenderComponent>();
	renderCoily->SetTexture("../Resources/Sprites/CoilyEgg.png");

	//Movemement
	coily->AddComponent<dae::CoilyComponent>(level1Component);
	
	scene.Add(std::move(coily));


	//Qbert Commands
	auto upLeftQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(1,1));
	dae::InputManager::GetInstance().BindCommand(SDLK_w, std::move(upLeftQbertCommand),true);

	auto upRightQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(-1, 1));
	dae::InputManager::GetInstance().BindCommand(SDLK_a, std::move(upRightQbertCommand), true);

	auto downLeftQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(-1, -1));
	dae::InputManager::GetInstance().BindCommand(SDLK_s, std::move(downLeftQbertCommand), true);

	auto downRightQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(1, -1));
	dae::InputManager::GetInstance().BindCommand(SDLK_d, std::move(downRightQbertCommand), true);

	//Add controls explanation
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("Use WASD to move Qbert"
		, font);
	go->SetLocalPosition(glm::vec3(10, 420, 0));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("Moving and falling of the level plays a sound"
		, font);
	go->SetLocalPosition(glm::vec3(10, 440, 0));
	scene.Add(std::move(go));
}

int main(int, char* [])
{
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}