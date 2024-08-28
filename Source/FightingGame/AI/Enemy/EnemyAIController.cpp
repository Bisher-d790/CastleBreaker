#include "EnemyAIController.h"

// Engine
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"

// Project
#include "FightingGame/AI/Enemy/EnemyCharacter.h"
#include "FightingGame/Data/EnemySettings.h"
#include "FightingGame/Items/Weapons/WeaponItem.h"


void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(AttackingWeapon))
	{
		if (!AttackingWeapon->IsAttacking())
		{
			FinishAttack();
		}
	}
}

void AEnemyAIController::SetupSettingsFromDT()
{
	Super::SetupSettingsFromDT();

	if (!SettingsTable || !IsValid(Blackboard)) return;

	const auto EnemySettings = SettingsTable->FindRow<FEnemySettings>(FName(SettingsRow), SettingsRow);
	if (!EnemySettings) return;

	Blackboard->SetValueAsFloat(PatrolRadiusBlackboard, EnemySettings->PatrolRadius);
	Blackboard->SetValueAsFloat(MinCombatRangeBlackboard, EnemySettings->MinCombatRange);
	Blackboard->SetValueAsFloat(MaxCombatRangeBlackboard, EnemySettings->MaxCombatRange);
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
	SetFocus(Enemy, EAIFocusPriority::Gameplay);
}

void AEnemyAIController::StartAttack()
{
	if (const auto OwnerCharacter = GetPawn<AEnemyCharacter>())
	{
		// Get the equipped weapon
		AttackingWeapon = OwnerCharacter->GetEquippedItem<AWeaponItem>();

		if (IsValid(AttackingWeapon))
		{
			// Start attack
			OwnerCharacter->StartPrimaryAction();

			// Stop the primary action to avoid issues
			OwnerCharacter->StopPrimaryAction();
		}
	}
}

void AEnemyAIController::FinishAttack()
{
	AttackingWeapon = nullptr;
}

bool AEnemyAIController::IsAttacking() const
{
	return (IsValid(AttackingWeapon) && AttackingWeapon->IsAttacking());
}