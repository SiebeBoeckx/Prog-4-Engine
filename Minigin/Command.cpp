#include "Command.h"

using namespace dae;

MoveCommand::MoveCommand(std::shared_ptr<GameObject> object, float speed, glm::f32vec2 dir)
	:m_pObj(object)
	,m_Speed(speed)
	,m_Dir(dir)
{
}

void MoveCommand::Execute(float deltaTime)
{
	glm::vec2 currentPos{ m_pObj.lock()->GetWorldPosition().x, m_pObj.lock()->GetWorldPosition().y };

	// move game object
	currentPos.x += m_Speed * m_Dir.x * deltaTime;
	currentPos.y += m_Speed * m_Dir.y * deltaTime;
	m_pObj.lock()->SetLocalPosition(glm::vec3(currentPos.x, currentPos.y, m_pObj.lock()->GetWorldPosition().z));
	m_pObj.lock()->SetPositionDirty();
}

void MoveCommand::Undo(float deltaTime)
{
	const auto objTransform{ m_pObj.lock()->GetComponent<Transform>() };
	glm::vec2 currentPos{ objTransform->GetPosition().x, objTransform->GetPosition().y };

	// un-move game object
	currentPos.x -= m_Speed * m_Dir.x * deltaTime;
	currentPos.y -= m_Speed * m_Dir.y * deltaTime;
	objTransform->SetPosition(currentPos.x, currentPos.y, objTransform->GetPosition().z);
}