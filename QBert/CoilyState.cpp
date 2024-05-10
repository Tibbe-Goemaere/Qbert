#include "CoilyState.h"
#include "CoilyComponent.h"

std::unique_ptr<dae::CoilyState> dae::CoilyState::EggState = std::make_unique<dae::EggState>();
std::unique_ptr<dae::CoilyState> dae::CoilyState::TransformingState = std::make_unique<dae::TransformingState>();
std::unique_ptr<dae::CoilyState> dae::CoilyState::SnakeState = std::make_unique<dae::SnakeState>();
std::unique_ptr<dae::CoilyState> dae::CoilyState::DyingState = std::make_unique<dae::DyingState>();

std::unique_ptr<dae::CoilyState> dae::EggState::Update(CoilyComponent*)
{
	if (m_pEggMovement->IsAtBottom())
	{
		auto pLevel = m_pEggMovement->GetLevel();
		auto pCurrentBlock = m_pEggMovement->GetCurrentBlock();
		m_pParent->RemoveComponent<MoveDownComponent>();
		m_pCoilyMovement = m_pParent->AddComponent<FollowPlayerComponent>(pLevel, pCurrentBlock->row, pCurrentBlock->column);
		SetTexture("Resources/Sprites/Coily.png");
		return std::move(dae::CoilyState::TransformingState);
	}
	return nullptr;
}

void dae::CoilyState::OnEnter()
{

}

std::unique_ptr<dae::CoilyState> dae::TransformingState::Update(CoilyComponent*)
{
	return nullptr;
}

std::unique_ptr<dae::CoilyState> dae::SnakeState::Update(CoilyComponent*)
{
	return nullptr;
}

std::unique_ptr<dae::CoilyState> dae::DyingState::Update(CoilyComponent*)
{
	return nullptr;
}


