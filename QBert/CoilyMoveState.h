#pragma once
#include <memory>

namespace dae
{
	class MoveComponent;
	class CoilyComponent;

	class CoilyMoveState
	{
	public:
		static std::unique_ptr<CoilyMoveState> EggState;
		static std::unique_ptr<CoilyMoveState> SnakeState;
		static std::unique_ptr<CoilyMoveState> DyingState;

		virtual ~CoilyMoveState() = default;
		CoilyMoveState(const CoilyMoveState& other) = delete;
		CoilyMoveState(CoilyMoveState&& other) = delete;
		CoilyMoveState& operator=(const CoilyMoveState& other) = delete;
		CoilyMoveState& operator=(CoilyMoveState&& other) = delete;

		virtual std::unique_ptr<CoilyMoveState> Update(CoilyComponent*) { return nullptr; };
		virtual void OnEnter(CoilyComponent*) {}
		virtual void OnExit(CoilyComponent*) {}
	protected:
		explicit CoilyMoveState() = default;
	};

	class EggState final : public CoilyMoveState
	{
	public:
		EggState();
		virtual ~EggState() {}
		virtual std::unique_ptr<CoilyMoveState> Update(CoilyComponent* pComponent) override;
		virtual void OnEnter(CoilyComponent*) override;
		virtual void OnExit(CoilyComponent*) override {};
	private:
		float m_timer;
		const float m_waitTime;
		MoveComponent* m_pMoveComponent;
	};


	class SnakeState final : public CoilyMoveState
	{
	public:
		virtual ~SnakeState() {}
		virtual std::unique_ptr<CoilyMoveState> Update(CoilyComponent* pComponent) override;
		virtual void OnEnter(CoilyComponent*) override;
		virtual void OnExit(CoilyComponent*) override {};
	};

	class DyingState final : public CoilyMoveState
	{
	public:
		virtual ~DyingState() {}
		virtual std::unique_ptr<CoilyMoveState> Update(CoilyComponent* pComponent) override;
		virtual void OnEnter(CoilyComponent*) override {};
		virtual void OnExit(CoilyComponent*) override {};
	};

}