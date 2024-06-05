#include "GameManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameIncludes.h"

dae::GameManager::GameManager(glm::vec2 windowSize)
	:m_scene{ SceneManager::GetInstance().CreateScene("MainMenu")}
	,m_windowSize{windowSize}
	,m_pFont{ResourceManager::GetInstance().LoadFont("Fonts/q-bert-original.otf",20)}
{
	MakeMenu();
}

void dae::GameManager::LoadLevel(const std::string&)
{
}

void dae::GameManager::MakeMenu()
{
	//Add UI
	const float marginBetweenText = 20.f;
	const float distanceFromArrow = 5.f;
	//Single player text
	auto singlePlayerText = std::make_unique<dae::GameObject>();
	auto renderComp = singlePlayerText->AddComponent<dae::RenderComponent>();
	singlePlayerText->AddComponent<dae::TextComponent>("Single Player", m_pFont);
	auto textSize = renderComp->GetTextTextureSize();
	singlePlayerText->SetLocalPosition(glm::vec3(m_windowSize.x/2.f - textSize.x/2.f, m_windowSize.y / 2.f - textSize.y/2.f - marginBetweenText, 0));
	m_scene.Add(std::move(singlePlayerText));

	//Add arrows around text
	auto leftArrow = std::make_unique<dae::GameObject>();
	renderComp = leftArrow->AddComponent<dae::RenderComponent>();
	renderComp->SetTexture("Sprites/UI/LeftArrow.png");
	auto arrowSize = renderComp->GetTextureSize();
	leftArrow->SetLocalPosition(glm::vec3(m_windowSize.x / 2.f + textSize.x / 2.f + distanceFromArrow + arrowSize.x, m_windowSize.y / 2.f - arrowSize.y / 2.f - marginBetweenText, 0));
	m_scene.Add(std::move(leftArrow));
	//Right arrow
	auto rightArrow = std::make_unique<dae::GameObject>();
	renderComp = rightArrow->AddComponent<dae::RenderComponent>();
	renderComp->SetTexture("Sprites/UI/RightArrow.png");
	rightArrow->SetLocalPosition(glm::vec3(m_windowSize.x / 2.f - textSize.x / 2.f - arrowSize.x - distanceFromArrow, m_windowSize.y / 2.f - arrowSize.y / 2.f - marginBetweenText, 0));
	m_scene.Add(std::move(rightArrow));
	

	//Co op text
	auto coopText = std::make_unique<dae::GameObject>();
	renderComp = coopText->AddComponent<dae::RenderComponent>();
	coopText->AddComponent<dae::TextComponent>("Co op", m_pFont);
	textSize = renderComp->GetTextTextureSize();
	coopText->SetLocalPosition(glm::vec3(m_windowSize.x / 2.f - textSize.x / 2.f, m_windowSize.y / 2.f, 0));
	m_scene.Add(std::move(coopText));

	//Add arrows around text
	leftArrow = std::make_unique<dae::GameObject>();
	renderComp = leftArrow->AddComponent<dae::RenderComponent>();
	renderComp->SetTexture("Sprites/UI/LeftArrow.png");
	leftArrow->SetLocalPosition(glm::vec3(m_windowSize.x / 2.f + textSize.x / 2.f + distanceFromArrow, m_windowSize.y / 2.f + (textSize.y / 2.f - arrowSize.y / 2.f), 0));
	m_scene.Add(std::move(leftArrow));
	//Right arrow
	rightArrow = std::make_unique<dae::GameObject>();
	renderComp = rightArrow->AddComponent<dae::RenderComponent>();
	renderComp->SetTexture("Sprites/UI/RightArrow.png");
	rightArrow->SetLocalPosition(glm::vec3(m_windowSize.x / 2.f - textSize.x / 2.f - arrowSize.x - distanceFromArrow, m_windowSize.y / 2.f + (textSize.y / 2.f - arrowSize.y / 2.f), 0));
	m_scene.Add(std::move(rightArrow));


	//Versus Text
	auto versusText = std::make_unique<dae::GameObject>();
	renderComp = versusText->AddComponent<dae::RenderComponent>();
	versusText->AddComponent<dae::TextComponent>("Versus", m_pFont);
	textSize = renderComp->GetTextTextureSize();
	versusText->SetLocalPosition(glm::vec3(m_windowSize.x / 2.f - textSize.x / 2.f, m_windowSize.y / 2.f + textSize.y / 2.f + marginBetweenText, 0));
	m_scene.Add(std::move(versusText));

	//Add arrows around text
	leftArrow = std::make_unique<dae::GameObject>();
	renderComp = leftArrow->AddComponent<dae::RenderComponent>();
	renderComp->SetTexture("Sprites/UI/LeftArrow.png");
	leftArrow->SetLocalPosition(glm::vec3(m_windowSize.x / 2.f + textSize.x / 2.f + distanceFromArrow, m_windowSize.y / 2.f + arrowSize.y / 2.f + marginBetweenText + arrowSize.y / 2.f, 0));
	m_scene.Add(std::move(leftArrow));
	//Right arrow
	rightArrow = std::make_unique<dae::GameObject>();
	renderComp = rightArrow->AddComponent<dae::RenderComponent>();
	renderComp->SetTexture("Sprites/UI/RightArrow.png");
	rightArrow->SetLocalPosition(glm::vec3(m_windowSize.x / 2.f - textSize.x / 2.f - arrowSize.x - distanceFromArrow, m_windowSize.y / 2.f + arrowSize.y /2.f + marginBetweenText + (arrowSize.y / 2.f), 0));
	m_scene.Add(std::move(rightArrow));
}
