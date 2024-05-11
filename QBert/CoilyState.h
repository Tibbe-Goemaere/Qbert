#pragma once
#include <memory>

namespace dae
{
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
		virtual void OnEnter(CoilyComponent*) {}
	protected:
		explicit CoilyState() = default;
	};

	class EggState final : public CoilyState
	{
	public:
		virtual ~EggState() {}
		virtual std::unique_ptr<CoilyState> Update(CoilyComponent* pComponent) override;
		virtual void OnEnter(CoilyComponent*) override {};
	};


	class SnakeState final : public CoilyState
	{
	public:
		virtual ~SnakeState() {}
		virtual std::unique_ptr<CoilyState> Update(CoilyComponent* pComponent) override;
		virtual void OnEnter(CoilyComponent*) override;
	};

	class DyingState final : public CoilyState
	{
	public:
		virtual ~DyingState() {}
		virtual std::unique_ptr<CoilyState> Update(CoilyComponent* pComponent) override;
	};

}
