#include "GameManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameIncludes.h"

dae::GameManager::GameManager(glm::vec2 windowSize)
	:m_windowSize{windowSize}
	,m_pFont{ResourceManager::GetInstance().LoadFont("Fonts/q-bert-original.otf",20)}
	,m_currentGameMode{GameMode::SinglePlayer}
{
	//MakeMenu();
	MakeSinglePlayerLevel(0);
	LoadLevel(m_singlePlayerLevelNames[0]);
}

void dae::GameManager::LoadLevel(const std::string& levelName)
{
	SceneManager::GetInstance().PickScene(levelName);
}

void dae::GameManager::MakeMenu()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("MainMenu");
	//Add UI
	const float marginBetweenText = 20.f;
	const float distanceFromArrow = 5.f;
	//Single player text
	auto singlePlayerText = std::make_unique<dae::GameObject>();
	auto renderComp = singlePlayerText->AddComponent<dae::RenderComponent>();
	singlePlayerText->AddComponent<dae::TextComponent>("Single Player", m_pFont);
	auto textSize = renderComp->GetTextTextureSize();
	singlePlayerText->SetLocalPosition(glm::vec3(m_windowSize.x/2.f - textSize.x/2.f, m_windowSize.y / 2.f - textSize.y/2.f - marginBetweenText, 0));
	scene.Add(std::move(singlePlayerText));

	//Vector of arrow render components fr commands
	std::vector<dae::RenderComponent*> m_pArrowRenderComponents{};
	//Add arrows around text
	auto arrows = std::make_unique<dae::GameObject>();
	renderComp = arrows->AddComponent<dae::RenderComponent>();
	renderComp->SetTexture("Sprites/UI/LeftArrow.png");
	auto arrowSize = renderComp->GetTextureSize();
	renderComp->SetTexture("Sprites/UI/RightArrow.png", true, { -textSize.x - arrowSize.x - distanceFromArrow * 2.f,0.f });
	arrows->SetLocalPosition(glm::vec3(m_windowSize.x / 2.f + textSize.x / 2.f + distanceFromArrow, m_windowSize.y / 2.f - arrowSize.y / 2.f - marginBetweenText, 0));
	scene.Add(std::move(arrows));
	m_pArrowRenderComponents.push_back(renderComp);


	//Co op text
	auto coopText = std::make_unique<dae::GameObject>();
	renderComp = coopText->AddComponent<dae::RenderComponent>();
	coopText->AddComponent<dae::TextComponent>("Co op", m_pFont);
	textSize = renderComp->GetTextTextureSize();
	coopText->SetLocalPosition(glm::vec3(m_windowSize.x / 2.f - textSize.x / 2.f, m_windowSize.y / 2.f, 0));
	scene.Add(std::move(coopText));

	//Add arrows around text
	arrows = std::make_unique<dae::GameObject>();
	renderComp = arrows->AddComponent<dae::RenderComponent>();
	renderComp->SetTexture("Sprites/UI/LeftArrow.png");
	renderComp->SetTexture("Sprites/UI/RightArrow.png", true, { -textSize.x - arrowSize.x - distanceFromArrow * 2.f,0.f });
	//renderComp->SetRenderTexture(false);
	arrows->SetLocalPosition(glm::vec3(m_windowSize.x / 2.f + textSize.x / 2.f + distanceFromArrow, m_windowSize.y / 2.f + (textSize.y / 2.f - arrowSize.y / 2.f), 0));
	scene.Add(std::move(arrows));
	m_pArrowRenderComponents.push_back(renderComp);


	//Versus Text
	auto versusText = std::make_unique<dae::GameObject>();
	renderComp = versusText->AddComponent<dae::RenderComponent>();
	versusText->AddComponent<dae::TextComponent>("Versus", m_pFont);
	textSize = renderComp->GetTextTextureSize();
	versusText->SetLocalPosition(glm::vec3(m_windowSize.x / 2.f - textSize.x / 2.f, m_windowSize.y / 2.f + textSize.y / 2.f + marginBetweenText, 0));
	scene.Add(std::move(versusText));

	//Add arrows around text
	arrows = std::make_unique<dae::GameObject>();
	renderComp = arrows->AddComponent<dae::RenderComponent>();
	renderComp->SetTexture("Sprites/UI/LeftArrow.png");
	renderComp->SetTexture("Sprites/UI/RightArrow.png", true, { -textSize.x - arrowSize.x - distanceFromArrow * 2.f,0.f });
	arrows->SetLocalPosition(glm::vec3(m_windowSize.x / 2.f + textSize.x / 2.f + distanceFromArrow, m_windowSize.y / 2.f + arrowSize.y / 2.f + marginBetweenText + arrowSize.y / 2.f, 0));
	scene.Add(std::move(arrows));
	m_pArrowRenderComponents.push_back(renderComp);

	//UI Object to manage where the arrows are and what the selection is
	auto chooseMenuOptions = std::make_unique<dae::GameObject>();
	auto uiComponent = chooseMenuOptions->AddComponent<dae::UIComponent>(m_pArrowRenderComponents);
	scene.Add(std::move(chooseMenuOptions));

	//Arrow moving up/down Commands
	auto upCommand = std::make_unique<dae::MoveArrows>(true, uiComponent);
	dae::InputManager::GetInstance().BindCommand(SDLK_w, std::move(upCommand), true);

	auto downCommand = std::make_unique<dae::MoveArrows>(false, uiComponent);
	dae::InputManager::GetInstance().BindCommand(SDLK_s, std::move(downCommand), true);

	//Choosing the gamemode command
	MakeSinglePlayerLevel(0);
	MakeCoopLevel(0);
	MakeVersusLevel(0);

	std::vector<std::string> m_firstLevels;
	m_firstLevels.push_back(m_singlePlayerLevelNames[0]);
	m_firstLevels.push_back(m_coopLevelNames[0]);
	m_firstLevels.push_back(m_versusLevelNames[0]);
	auto chooseCommand = std::make_unique<dae::ChooseGameMode>(m_firstLevels, uiComponent);


	dae::InputManager::GetInstance().BindCommand(SDLK_SPACE, std::move(chooseCommand), true);
}

void dae::GameManager::MakeSinglePlayerLevel(int idx)
{
	std::string sceneName = "Level1-" + std::to_string(idx + 1);
	auto& scene = dae::SceneManager::GetInstance().CreateScene(sceneName);
	m_singlePlayerLevelNames.push_back(sceneName);

	std::string levelPath = "../Data/Levels/" + sceneName + ".xml";
	// Add level
	auto level = std::make_unique<dae::GameObject>();
	level->AddComponent<dae::RenderComponent>();
	auto level1Component = level->AddComponent<dae::LevelComponent>("../Data/Levels/Level1-1.xml");
	level->SetLocalPosition({ 288,70,0 });
	scene.Add(std::move(level));

	//Add disks
	auto disk = std::make_unique<dae::GameObject>();
	disk->AddComponent<dae::DiskComponent>(level1Component, false, 1);
	scene.Add(std::move(disk));

	//Coily
	auto coily = std::make_unique<dae::GameObject>();

	coily->AddComponent<dae::CoilyComponent>(level1Component);

	//scene.Add(std::move(coily));

	//Slick
	auto slick = std::make_unique<dae::GameObject>();
	slick->AddComponent<dae::SlickComponent>(level1Component, "Sprites/Slick.png");
	scene.Add(std::move(slick));

	//Add controls explanation
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("Use WASD to move Qbert"
		, m_pFont);
	go->SetLocalPosition(glm::vec3(10, 420, 0));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("Moving and falling of the level plays a sound"
		, m_pFont);
	go->SetLocalPosition(glm::vec3(10, 440, 0));
	scene.Add(std::move(go));

	//Update manager + enemyspawner
	auto pEnemySpawner = std::make_unique<EnemySpawner>(level1Component, scene);
	//auto spawnInfo = std::make_unique<SpawnInfo>(EnemyType::Coily, 10.f, 20.f, 5.f);
	//pEnemySpawner->AddSpawn(std::move(spawnInfo));
	UpdateManager::GetInstance().AddUpdater(std::move(pEnemySpawner));

	//Ugg
	auto ugg = std::make_unique<dae::GameObject>();
	ugg->AddComponent<dae::UggComponent>(level1Component, "Sprites/Ugg.png", true);
	scene.Add(std::move(ugg));

	MakeQbert(level1Component, scene);
}

void dae::GameManager::MakeCoopLevel(int idx)
{
	std::string sceneName = "Level2-" + (idx + 1);
	dae::SceneManager::GetInstance().CreateScene(sceneName);
	m_coopLevelNames.push_back(sceneName);
}

void dae::GameManager::MakeVersusLevel(int idx)
{
	std::string sceneName = "Level3-" + (idx + 1);
	dae::SceneManager::GetInstance().CreateScene(sceneName);
	m_versusLevelNames.push_back(sceneName);
}

void dae::GameManager::MakeQbert(LevelComponent* pLevel, Scene& scene)
{
	//Add Qbert UI
	auto uiPointsObject = std::make_unique<dae::GameObject>();
	uiPointsObject->AddComponent<dae::RenderComponent>();
	auto pointsText = uiPointsObject->AddComponent<dae::TextComponent>("Points", m_pFont);
	uiPointsObject->SetLocalPosition(glm::vec3(20, 200, 0));
	scene.Add(std::move(uiPointsObject));

	auto uiLivesObject = std::make_unique<dae::GameObject>();
	auto healthComponent = uiLivesObject->AddComponent<dae::HealthComponent>();
	uiLivesObject->AddComponent<dae::HealthDisplayComponent>();
	uiLivesObject->SetLocalPosition(glm::vec3(20, 130, 0));
	scene.Add(std::move(uiLivesObject));

	//Qbert
	auto qbert = std::make_unique<dae::GameObject>();
	auto pQbert = scene.Add(std::move(qbert));

	//ScoreComponent
	pQbert->AddComponent<dae::ScoreComponent>();
	pQbert->AddComponent<dae::ScoreDisplayComponent>(pointsText);

	//QbertComponent
	pQbert->AddComponent<dae::QbertComponent>(healthComponent,pLevel);

	//Qbert Commands
	auto upLeftQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(1, 1));
	dae::InputManager::GetInstance().BindCommand(SDLK_w, std::move(upLeftQbertCommand), true);

	auto upRightQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(-1, 1));
	dae::InputManager::GetInstance().BindCommand(SDLK_a, std::move(upRightQbertCommand), true);

	auto downLeftQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(-1, -1));
	dae::InputManager::GetInstance().BindCommand(SDLK_s, std::move(downLeftQbertCommand), true);

	auto downRightQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(1, -1));
	dae::InputManager::GetInstance().BindCommand(SDLK_d, std::move(downRightQbertCommand), true);
}

void dae::GameManager::AddCoilySpawn()
{
}

void dae::GameManager::AddSlickSpawn()
{
}

void dae::GameManager::AddUggSpawn()
{
}
