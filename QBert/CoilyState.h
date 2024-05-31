#pragma once
#include <memory>
#include <glm/glm.hpp >

namespace dae
{
	class MoveComponent;
	class CoilyComponent;

	class CoilyState
	{
	public:
		static std::unique_ptr<CoilyState> EggState;
		static std::unique_ptr<CoilyState> SnakeState;
		static std::unique_ptr<CoilyState> DyingState;

		virtual ~CoilyState() = default;
		CoilyState(const CoilyState& other) = delete;
		CoilyState(CoilyState&& other) = delete;
		CoilyState& operator=(const CoilyState& other) = delete;
		CoilyState& operator=(CoilyState&& other) = delete;

		virtual std::unique_ptr<CoilyState> Update(CoilyComponent*) { return nullptr; };
		virtual void OnEnter(CoilyComponent* pCoilyComponent) = 0;
		virtual void OnExit(CoilyComponent* pCoilyComponent) = 0;
	protected:
		explicit CoilyState() = default;
	};

	class EggState final : public CoilyState
	{
	public:
		EggState();
		virtual ~EggState() {}
		virtual std::unique_ptr<CoilyState> Update(CoilyComponent* pComponent) override;
		virtual void OnEnter(CoilyComponent* pCoilyComponent) override;
		virtual void OnExit(CoilyComponent* pCoilyComponent) override;
	private:
		float m_timer;
		const float m_waitTime;
		MoveComponent* m_pMoveComponent;
	};


	class SnakeState final : public CoilyState
	{
	public:
		SnakeState();
		virtual ~SnakeState() {}
		virtual std::unique_ptr<CoilyState> Update(CoilyComponent* pComponent) override;
		virtual void OnEnter(CoilyComponent* pCoilyComponent) override;
		virtual void OnExit(CoilyComponent* pCoilyComponent) override;
	private:
		float m_timer;
		const float m_waitTime;
		MoveComponent* m_pMoveComponent;

		glm::vec2 FindNextBlock() const;
	};

	class DyingState final : public CoilyState
	{
	public:
		DyingState();
		virtual ~DyingState() {}
		virtual std::unique_ptr<CoilyState> Update(CoilyComponent* pComponent) override;
		virtual void OnEnter(CoilyComponent* pCoilyComponent) override;
		virtual void OnExit(CoilyComponent* pCoilyComponent) override;
	};

}