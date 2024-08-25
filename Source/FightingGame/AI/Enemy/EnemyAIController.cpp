#include "EnemyAIController.h"

// Engine
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"

// Project
#include "FightingGame/AI/Enemy/EnemyCharacter.h"


void AEnemyAIController::EnemyDetected(APawn* Enemy)
{
	if (!IsValid(Enemy)) return;

	// Is a valid player enemy
	const auto EnemyCharacter = Cast<AFGCharacter>(Enemy);
	if (!IsValid(EnemyCharacter)) return;

	SetTargetEnemy(EnemyCharacter);

	if (IsValid(Blackboard))
	{
		Blackboard->SetValueAsObject(TargetEnemyBlackboard, Enemy);
	}

	OnEnemyDetected.Broadcast(Enemy);
}
