#include "RotatingComponent.h"
#include "TimeManager.h"

dae::RotatingComponent::RotatingComponent(GameObject* pParent, float radius, float rotSpeed)
	:BaseComponent::BaseComponent(pParent)
	,m_radius{radius}
	,m_rotationSpeed{rotSpeed}
	,m_angle{0}
{}

dae::RotatingComponent::~RotatingComponent()
{

}

void dae::RotatingComponent::Awake()
{
	
}

void dae::RotatingComponent::Update()
{
	m_angle += (TimeManager::GetInstance().GetDeltaTime() * m_rotationSpeed);
	
	if (m_angle <= -360.f)
	{
		m_angle += 360.f;
	}
	else if (m_angle >= 360.f)
	{
		m_angle -= 360.f;
	}

	float x = m_radius * cosf(m_angle);
	float y = m_radius * sinf(m_angle);

	auto newPos = glm::vec3(m_pParent->GetLocalPosition().x + x, m_pParent->GetLocalPosition().y + y, 0.0f);
	m_pParent->SetLocalPosition(newPos);
}

