#include "EnemyAIController.h"

// Engine
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"

// Project
#include "FightingGame/AI/Enemy/EnemyCharacter.h"
#include "FightingGame/Data/EnemySettings.h"


void AEnemyAIController::SetupSettingsFromDT()
{
	Super::SetupSettingsFromDT();

	if (!SettingsTable || !IsValid(Blackboard)) return;

	const auto EnemySettings = SettingsTable->FindRow<FEnemySettings>(FName(SettingsRow), SettingsRow);
	if (!EnemySettings) return;


	Blackboard->SetValueAsFloat(PatrolRadiusBlackboard, EnemySettings->PatrolRadius);
}

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

void AEnemyAIController::SetTargetEnemy(APawn* Enemy)
{
	TargetEnemy = Enemy;
	SetFocus(TargetEnemy);
}
