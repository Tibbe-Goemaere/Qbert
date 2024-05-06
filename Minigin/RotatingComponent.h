#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "BaseComponent.h"

namespace dae
{
	class RotatingComponent final : public BaseComponent
	{
	public:
		void Awake() override;
		void Update() override;

		RotatingComponent(GameObject* pParent, float radius = 3.f, float rotSpeed = 8.f);
		virtual ~RotatingComponent();
		RotatingComponent(const RotatingComponent& other) = delete;
		RotatingComponent(RotatingComponent&& other) = delete;
		RotatingComponent& operator=(const RotatingComponent& other) = delete;
		RotatingComponent& operator=(RotatingComponent&& other) = delete;
	private:
		float m_radius;
		float m_rotationSpeed;
		float m_angle;
	};
}
