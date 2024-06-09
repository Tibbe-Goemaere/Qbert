#pragma once
#include "GameObject.h"
namespace dae
{
	class Command
	{
	public:
		virtual ~Command() {}
		virtual void Execute() = 0;
	};

	class MoveCommand final : public Command
	{
	public:
		MoveCommand(GameObject* pObject, const glm::vec3& direction, const float speed);

		void Execute() override;
	private:
		GameObject* m_pGameObject;
		const glm::vec3 m_Direction;
		const float m_Speed;
	};
}


