#include "GameComponents.h"

#include "CollisionManager.h"
#include "SceneManager.h"
#include "Enums.h"
#include "InputManager.h"
#include "LevelCreator.h"

#pragma region Player

PlayerComponent::PlayerComponent(dae::GameObject* owner, bool isGhost, int playerNr)
	:Component{ owner }
	,m_IsGhost{ isGhost }
	,m_PlayerNr{ playerNr }
	,m_StartPosition(owner->GetStartPosition())
{
	m_pPlayerSubject = std::make_unique<dae::Subject<Event>>();
}

void PlayerComponent::Start()
{
	m_pPlayerSubject->Notify(Event::PlayerStart);
}

void PlayerComponent::Update(float dt)
{
	auto deltaTime = dt;

	if (m_TargetDirection != m_Direction && !CollidesInTargetDir(dt))
	{
		m_Direction = m_TargetDirection;
	}

	auto pTransform = GetOwner()->GetComponent<dae::Transform>();
	glm::vec3 curPos = pTransform->GetPosition();
	glm::vec3 targetPos = curPos;

	float moveSpeedDeltaTime = m_MovementSpeed * deltaTime;

	switch (m_Direction)
	{
		// move to the right
	case Direction::RIGHT:
		targetPos.x += moveSpeedDeltaTime;
		break;
		// move to the left
	case Direction::LEFT:
		targetPos.x -= moveSpeedDeltaTime;
		break;
		// move down
	case Direction::DOWN:
		targetPos.y += moveSpeedDeltaTime;
		break;
		// move up
	case Direction::UP:
		targetPos.y -= moveSpeedDeltaTime;
		break;
	default:
		break;
	}

	const auto& collisions = dae::CollisionManager::GetInstance().GetColliders();
	const auto myCollider = this->GetOwner()->GetComponent<dae::ColliderComponent>();

	myCollider->SetPosition(targetPos.x, targetPos.y);

	// if the movement should be updated
	bool shouldUpdate = true;

	for (auto collision : collisions)
	{
		//skipping same collision
		if (collision == myCollider)
			continue;

		if (myCollider->IsColliding(collision))
		{
			if (collision->GetTag() == "PICKUP")
			{
				// code for triggering pickups like dots or powerups
				auto dot = collision->GetChangeableOwner();
				dot->RemoveComponent<dae::Transform>();
				dot->RemoveComponent<dae::RenderComponent>();
				dae::CollisionManager::GetInstance().RemoveCollider(dot->GetComponent<dae::ColliderComponent>());
				dot->RemoveComponent<dae::ColliderComponent>();
				dot->GetComponent<PacDotComponent>()->AddPoints();
				dot->RemoveComponent<PacDotComponent>();

				continue;
			}
			// if is colliding with something after an update, don't update movement!
			myCollider->SetPosition(curPos.x, curPos.y);
			shouldUpdate = false;
		}
	}

	if (shouldUpdate)
	{
		pTransform->SetPosition(targetPos);
	}

	GetChangeableOwner()->SetLocalPosition(pTransform->GetPosition());
}

void PlayerComponent::Initialize()
{
	m_pPlayerSubject->Notify(Event::PlayerStart);
}

void PlayerComponent::SetDirection(Direction direction)
{
	m_TargetDirection = direction;
}

void PlayerComponent::AddObserver(dae::Observer<Event>* obs)
{
	m_pPlayerSubject->AddObserver(obs);
}

void PlayerComponent::RemoveObserver(dae::Observer<Event>* obs)
{
	m_pPlayerSubject->RemoveObserver(obs);
}


void PlayerComponent::Respawn()
{
	SetPosition(m_StartPosition);
}

bool PlayerComponent::CollidesInTargetDir(float dt)
{
	auto pTransform = GetOwner()->GetComponent<dae::Transform>();
	glm::vec3 curPos = pTransform->GetPosition();
	glm::vec3 targetPos = curPos;

	// the amount the player should move independent of framerate aka multiplied
	// by deltaTime
	float moveSpeedDeltaTime = m_MovementSpeed * dt;

	switch (m_TargetDirection)
	{
		// move to the right
	case Direction::RIGHT:
		targetPos.x += moveSpeedDeltaTime;
		break;
		// move to the left
	case Direction::LEFT:
		targetPos.x -= moveSpeedDeltaTime;
		break;
		// move down
	case Direction::DOWN:
		targetPos.y += moveSpeedDeltaTime;
		break;
		// move up
	case Direction::UP:
		targetPos.y -= moveSpeedDeltaTime;
		break;
	default:
		break;
	}

	const auto& collisions = dae::CollisionManager::GetInstance().GetColliders();
	const auto myCollider = this->GetOwner()->GetComponent<dae::ColliderComponent>();

	// set the collider position to the future movement to test collision
	myCollider->SetPosition(targetPos.x, targetPos.y);

	for (const auto collision : collisions)
	{
		//skipping same collision
		if (collision == myCollider)
			continue;

		// if collides in future, return true
		if (myCollider->IsColliding(collision) && collision->GetTag() != "PICKUP")
		{
			myCollider->SetPosition(curPos.x, curPos.y);
			return true;
		}

	}

	myCollider->SetPosition(curPos.x, curPos.y);
	// else no collision in future target dir, return false
	return false;
}

void PlayerComponent::Die()
{
	if (m_Lives > 0)
	{
		--m_Lives;
	}
	m_pPlayerSubject->Notify(Event::PlayerDied);
}

#pragma endregion

#pragma region Dots

PacDotComponent::PacDotComponent(dae::GameObject* owner)
	: Component{ owner }
{
}

PacDotComponent::~PacDotComponent()
{
}

void PacDotComponent::AddPoints()
{
	const auto player = dae::LevelCreator::GetInstance().GetPlayer1();
	player->GetComponent<PlayerComponent>()->AddScore(m_PointAmnt);
}

#pragma endregion