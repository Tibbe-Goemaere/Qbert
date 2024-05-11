#include "CoilyState.h"
#include "CoilyComponent.h"

std::unique_ptr<dae::CoilyState> dae::CoilyState::EggState = std::make_unique<dae::EggState>();
std::unique_ptr<dae::CoilyState> dae::CoilyState::SnakeState = std::make_unique<dae::SnakeState>();
std::unique_ptr<dae::CoilyState> dae::CoilyState::DyingState = std::make_unique<dae::DyingState>();

std::unique_ptr<dae::CoilyState> dae::EggState::Update(CoilyComponent* pCoilyComponent)
{
	if (pCoilyComponent->IsAtBottom())
	{
		return std::move(CoilyState::SnakeState);
	}
	return nullptr;
}

std::unique_ptr<dae::CoilyState> dae::SnakeState::Update(CoilyComponent* pCoilyComponent)
{
	if (pCoilyComponent->IsDead())
	{
		//return std::move(CoilyState::DyingState);
	}
	return nullptr;
}

void dae::SnakeState::OnEnter(CoilyComponent* pCoilyComponent)
{
	pCoilyComponent->ChangeToSnake();
}

std::unique_ptr<dae::CoilyState> dae::DyingState::Update(CoilyComponent*)
{
	return nullptr;
}


