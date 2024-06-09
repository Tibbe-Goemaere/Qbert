#include "GameManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameIncludes.h"
#include <iostream>

dae::GameManager::GameManager()
	:m_windowSize{ glm::vec2{19020,1080} }
	,m_pFont{ResourceManager::GetInstance().LoadFont("Fonts/q-bert-original.otf",20)}
	,m_currentGameMode{GameMode::SinglePlayer}
	,m_pCoilySpawnInfo{std::make_shared<SpawnInfo>(EnemyType::Coily,30.f,35.f,2.f)}
	,m_pSlickSpawnInfo{ std::make_shared<SpawnInfo>(EnemyType::Slick,13.f,17.f,5.f) }
	,m_pUggSpawnInfo{ std::make_shared<SpawnInfo>(EnemyType::Ugg,12.f,16.f,5.f) }
	,m_amountOfLevels{3}
	,m_menuName{"MainMenu"}
	,m_currentLevelIdx{1}
{
	auto skipLevelCommand = std::make_unique<dae::SkipLevelCommand>();
	dae::InputManager::GetInstance().BindCommand(SDLK_F1, std::move(skipLevelCommand), true);

	//MakeMenu();
	MakeSinglePlayerLevel(m_currentLevelIdx);
}

void dae::GameManager::SetWindowSize(glm::vec2 windowSize)
{
	m_windowSize = windowSize;
}

void dae::GameManager::LoadLevel(const std::string& levelName)
{
	SceneManager::GetInstance().PickScene(levelName);
}

void dae::GameManager::GoToNextLevel()
{
	InputManager::GetInstance().UnregisterCommands();

	auto skipLevelCommand = std::make_unique<dae::SkipLevelCommand>();
	dae::InputManager::GetInstance().BindCommand(SDLK_F1, std::move(skipLevelCommand), true);

	++m_currentLevelIdx;
	switch (m_currentGameMode)
	{
	case dae::GameMode::SinglePlayer:
		MakeSinglePlayerLevel(m_currentLevelIdx);
		break;
	case dae::GameMode::Coop:
		MakeCoopLevel(m_currentLevelIdx);
		break;
	case dae::GameMode::Versus:
		MakeVersusLevel(m_currentLevelIdx);
		break;
	default:
		break;
	}
}

int dae::GameManager::GetCurrentLevelIdx() const
{
	return m_currentLevelIdx;
}

void dae::GameManager::SetAmountOfPoints(int amountOfPoints, int playerIdx)
{
	if (playerIdx == 0)
	{
		m_amountOfPointsPlayer1 = amountOfPoints;
	}
	else
	{
		m_amountOfPointsPlayer2 = amountOfPoints;
	}
}

void dae::GameManager::SetGameMode(GameMode newGameMode)
{
	m_currentGameMode = newGameMode;
}

void dae::GameManager::RemoveCoilyCommands()
{
	auto& inputManager = InputManager::GetInstance();
	for (const auto& pCommand : m_pCoilyCommands)
	{
		inputManager.UnregisterCommand(pCommand);
	}
	m_pCoilyCommands.clear();
}

void dae::GameManager::MakeCoilyPlayer(LevelComponent* pLevel)
{
	auto& scene = SceneManager::GetInstance().GetCurrentScene();
	MakeCoily(pLevel, scene);
}

void dae::GameManager::MakeMenu()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene(m_menuName);
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
	//MakeSinglePlayerLevel(0);
	//MakeCoopLevel(0);
	//MakeVersusLevel(0);

	std::vector<std::string> m_firstLevels;
	auto chooseCommand = std::make_unique<dae::ChooseGameMode>(uiComponent);


	dae::InputManager::GetInstance().BindCommand(SDLK_SPACE, std::move(chooseCommand), true);
}

void dae::GameManager::MakeSinglePlayerLevel(int idx)
{
	if (idx > m_amountOfLevels)
	{
		SceneManager::GetInstance().PickScene(m_menuName);
		return;
	}

	const std::string currentLevelName = "Level1-" + std::to_string(idx);
	const std::string currentLevelPath = "../Data/Levels/" + currentLevelName + ".xml";

	auto& scene = dae::SceneManager::GetInstance().CreateScene(currentLevelName);

	// Add level
	auto level = std::make_unique<dae::GameObject>();
	level->AddComponent<dae::RenderComponent>();
	auto levelComponent = level->AddComponent<dae::LevelComponent>(currentLevelPath);
	level->SetLocalPosition({ 288,70,0 });
	scene.Add(std::move(level));

	MakeSpawns(levelComponent, scene);
	MakeDisks(levelComponent, scene);
	MakeQbert(levelComponent, scene);

	dae::SceneManager::GetInstance().PickScene(currentLevelName);
}

void dae::GameManager::MakeCoopLevel(int idx)
{
	if (idx > m_amountOfLevels)
	{
		SceneManager::GetInstance().PickScene(m_menuName);
		return;
	}

	const std::string currentLevelName = "Level2-" + std::to_string(idx);
	const std::string currentLevelPath = "../Data/Levels/" + currentLevelName + ".xml";

	auto& scene = dae::SceneManager::GetInstance().CreateScene(currentLevelName);

	// Add level
	auto level = std::make_unique<dae::GameObject>();
	level->AddComponent<dae::RenderComponent>();
	auto levelComponent = level->AddComponent<dae::LevelComponent>(currentLevelPath);
	level->SetLocalPosition({ 288,70,0 });
	scene.Add(std::move(level));

	MakeSpawns(levelComponent, scene);
	MakeDisks(levelComponent, scene);
	const int lowestRow = levelComponent->GetAmountOfSteps() - 1;
	MakeQbert(levelComponent, scene,{ lowestRow,0});
	MakeQbert(levelComponent, scene, { lowestRow,lowestRow },true);

	dae::SceneManager::GetInstance().PickScene(currentLevelName);
}

void dae::GameManager::MakeVersusLevel(int idx)
{
	if (idx > m_amountOfLevels)
	{
		SceneManager::GetInstance().PickScene(m_menuName);
		return;
	}

	const std::string currentLevelName = "Level3-" + std::to_string(idx);
	const std::string currentLevelPath = "../Data/Levels/" + currentLevelName + ".xml";

	auto& scene = dae::SceneManager::GetInstance().CreateScene(currentLevelName);

	// Add level
	auto level = std::make_unique<dae::GameObject>();
	level->AddComponent<dae::RenderComponent>();
	auto levelComponent = level->AddComponent<dae::LevelComponent>(currentLevelPath);
	level->SetLocalPosition({ 288,70,0 });
	scene.Add(std::move(level));

	MakeSpawns(levelComponent, scene);
	MakeDisks(levelComponent, scene);
	MakeQbert(levelComponent, scene);

	MakeCoily(levelComponent, scene);

	dae::SceneManager::GetInstance().PickScene(currentLevelName);
}

void dae::GameManager::MakeQbert(LevelComponent* pLevel, Scene& scene, const std::pair<int, int> gridPos, bool isSecondPlayer)
{
	//Add Qbert UI
	auto uiPointsObject = std::make_unique<dae::GameObject>();
	uiPointsObject->AddComponent<dae::RenderComponent>();
	auto pointsText = uiPointsObject->AddComponent<dae::TextComponent>("Points", m_pFont);
	if (isSecondPlayer)
	{
		uiPointsObject->SetLocalPosition(glm::vec3(450, 100, 0));
	}
	else
	{
		uiPointsObject->SetLocalPosition(glm::vec3(20, 100, 0));
	}
	scene.Add(std::move(uiPointsObject));

	auto uiLivesObject = std::make_unique<dae::GameObject>();
	auto healthComponent = uiLivesObject->AddComponent<dae::HealthComponent>();
	uiLivesObject->AddComponent<dae::HealthDisplayComponent>();
	if (isSecondPlayer)
	{
		uiLivesObject->SetLocalPosition(glm::vec3(600, 130, 0));
	}
	else
	{
		uiLivesObject->SetLocalPosition(glm::vec3(20, 130, 0));
	}
	scene.Add(std::move(uiLivesObject));

	//Qbert
	auto qbert = std::make_unique<dae::GameObject>();
	auto pQbert = scene.Add(std::move(qbert));

	//ScoreComponent
	int startPoints = m_amountOfPointsPlayer1;
	if (isSecondPlayer)
	{
		startPoints = m_amountOfPointsPlayer2;
	}
	pQbert->AddComponent<dae::ScoreComponent>(startPoints);
	pQbert->AddComponent<dae::ScoreDisplayComponent>(pointsText);

	//QbertComponent
	pQbert->AddComponent<dae::QbertComponent>(healthComponent,pLevel, gridPos,isSecondPlayer);

	//Qbert Commands
	if (!isSecondPlayer)
	{
		auto upLeftQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(1, 1));
		dae::InputManager::GetInstance().BindCommand(SDLK_w, std::move(upLeftQbertCommand), true);

		auto upRightQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(-1, 1));
		dae::InputManager::GetInstance().BindCommand(SDLK_a, std::move(upRightQbertCommand), true);

		auto downLeftQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(-1, -1));
		dae::InputManager::GetInstance().BindCommand(SDLK_s, std::move(downLeftQbertCommand), true);

		auto downRightQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(1, -1));
		dae::InputManager::GetInstance().BindCommand(SDLK_d, std::move(downRightQbertCommand), true);
	}
	else if (isSecondPlayer || m_currentGameMode == GameMode::SinglePlayer)
	{
		auto upLeftQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(1, 1));
		dae::InputManager::GetInstance().BindCommand(Controller::ControllerButton::DPadUp, std::move(upLeftQbertCommand), true);

		auto upRightQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(-1, 1));
		dae::InputManager::GetInstance().BindCommand(Controller::ControllerButton::DPadRight, std::move(upRightQbertCommand), true);

		auto downLeftQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(-1, -1));
		dae::InputManager::GetInstance().BindCommand(Controller::ControllerButton::DPadDown, std::move(downLeftQbertCommand), true);

		auto downRightQbertCommand = std::make_unique<dae::MovePlayerCommand>(pQbert, glm::vec2(1, -1));
		dae::InputManager::GetInstance().BindCommand(Controller::ControllerButton::DPadLeft, std::move(downRightQbertCommand), true);
	}
	
}

void dae::GameManager::MakeSpawns(LevelComponent* pLevel, Scene& scene)
{
	UpdateManager::GetInstance().RemoveAllUpdaters();

	//Update manager + enemyspawner
	auto pEnemySpawner = std::make_unique<EnemySpawner>(pLevel, scene);
	const auto levelInfo = pLevel->GetLevelInfo();
	if (levelInfo->hasCoily)
	{
		pEnemySpawner->AddSpawn(m_pCoilySpawnInfo);
	}
	if (levelInfo->hasSlick)
	{
		pEnemySpawner->AddSpawn(m_pSlickSpawnInfo);
	}
	if (levelInfo->hasUgg)
	{
		pEnemySpawner->AddSpawn(m_pUggSpawnInfo);
	}

	UpdateManager::GetInstance().AddUpdater(std::move(pEnemySpawner));
}

void dae::GameManager::MakeDisks(LevelComponent* pLevel, Scene& scene)
{
	//Add disks left and right at random rows
	int randomRow = rand() % pLevel->GetAmountOfSteps();

	auto disk = std::make_unique<dae::GameObject>();
	disk->AddComponent<dae::DiskComponent>(pLevel, true, randomRow);
	scene.Add(std::move(disk));

	randomRow = rand() % pLevel->GetAmountOfSteps();
	disk = std::make_unique<dae::GameObject>();
	disk->AddComponent<dae::DiskComponent>(pLevel, false, randomRow);
	scene.Add(std::move(disk));
}

void dae::GameManager::MakeCoily(LevelComponent* pLevel, Scene& scene)
{
	//Adding Playable Coily 
	auto coily = std::make_unique<dae::GameObject>();
	coily->AddComponent<dae::CoilyComponent>(pLevel);
	auto pCoily = scene.Add(std::move(coily));

	//Coily Commands
	auto upLeftCoilyCommand = std::make_unique<dae::MoveCoilyCommand>(pCoily, glm::vec2(1, 1));
	m_pCoilyCommands.emplace_back(upLeftCoilyCommand.get());
	dae::InputManager::GetInstance().BindCommand(Controller::ControllerButton::DPadUp, std::move(upLeftCoilyCommand), true);

	auto upRightCoilyCommand = std::make_unique<dae::MoveCoilyCommand>(pCoily, glm::vec2(-1, 1));
	m_pCoilyCommands.emplace_back(upRightCoilyCommand.get());
	dae::InputManager::GetInstance().BindCommand(Controller::ControllerButton::DPadLeft, std::move(upRightCoilyCommand), true);

	auto downLeftCoilyCommand = std::make_unique<dae::MoveCoilyCommand>(pCoily, glm::vec2(-1, -1));
	m_pCoilyCommands.emplace_back(downLeftCoilyCommand.get());
	dae::InputManager::GetInstance().BindCommand(Controller::ControllerButton::DPadDown, std::move(downLeftCoilyCommand), true);

	auto downRightCoilyCommand = std::make_unique<dae::MoveCoilyCommand>(pCoily, glm::vec2(1, -1));
	m_pCoilyCommands.emplace_back(downRightCoilyCommand.get());
	dae::InputManager::GetInstance().BindCommand(Controller::ControllerButton::DPadRight, std::move(downRightCoilyCommand), true);
}
