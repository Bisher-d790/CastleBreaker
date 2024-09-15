#include "EnemyAIController.h"

// Engine
#include "BehaviorTree/BlackboardComponent.h"

// Project
#include "CastleBreaker/AI/Enemy/EnemyCharacter.h"
#include "CastleBreaker/Components/HealthComponent.h"
#include "CastleBreaker/Data/EnemySettings.h"
#include "CastleBreaker/Items/Weapons/WeaponItem.h"


void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If there's a weapon currently in use for attack, check if it has finished
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

	// Setup settings from Data Table
	bOnlyDamagePlayers = EnemySettings->bOnlyCanDamagePlayers;
	LowHealthThreshold = EnemySettings->LowHealthThreshold;

	Blackboard->SetValueAsFloat(MinPatrolRadiusBlackboard, EnemySettings->MinPatrolRadius);
	Blackboard->SetValueAsFloat(MaxPatrolRadiusBlackboard, EnemySettings->MaxPatrolRadius);
	Blackboard->SetValueAsFloat(MinCombatRangeBlackboard, EnemySettings->MinCombatRange);
	Blackboard->SetValueAsFloat(MaxCombatRangeBlackboard, EnemySettings->MaxCombatRange);
	Blackboard->SetValueAsFloat(AttackDistanceBlackboard, EnemySettings->AttackDistance);
	Blackboard->SetValueAsInt(MaxAttackersBlackboard, EnemySettings->MaxConcurrentAttackers);
}

void AEnemyAIController::EnemyDetected(APawn* Enemy)
{
	if (!IsValid(Enemy)) return;

	// Is a valid player enemy
	const auto EnemyCharacter = Cast<ACBCharacter>(Enemy);
	if (!IsValid(EnemyCharacter)) return;

	const auto EnemyHealthComponent = EnemyCharacter->GetHealthComponent();
	// Don't detect dead enemies
	if (!IsValid(EnemyHealthComponent) || EnemyHealthComponent->IsDead()) return;

	SetTargetEnemy(EnemyCharacter);

	// Undetect enemy after it has been killed
	if (!EnemyHealthComponent->OnDeath.IsAlreadyBound(this, &AEnemyAIController::EnemyUnDetected))
		EnemyHealthComponent->OnDeath.AddDynamic(this, &AEnemyAIController::EnemyUnDetected);

	OnEnemyDetected.Broadcast(Enemy);
}

void AEnemyAIController::EnemyUnDetected()
{
	if (IsValid(TargetEnemy) && Cast<ACBCharacter>(TargetEnemy))
	{
		if (const auto EnemyHealthComponent = Cast<ACBCharacter>(TargetEnemy)->GetHealthComponent())
		{
			if (EnemyHealthComponent->OnDeath.IsAlreadyBound(this, &AEnemyAIController::EnemyUnDetected))
				EnemyHealthComponent->OnDeath.RemoveDynamic(this, &AEnemyAIController::EnemyUnDetected);
		}
	}

	// Remove the current target reference
	SetTargetEnemy(nullptr);

	OnEnemyUnDetected.Broadcast();
}

void AEnemyAIController::SetTargetEnemy(APawn* Enemy)
{
	TargetEnemy = Enemy;

	SetFocus(TargetEnemy, EAIFocusPriority::Gameplay);

	// Set the new target in the Blackboard
	if (IsValid(Blackboard))
	{
		Blackboard->SetValueAsObject(TargetEnemyBlackboard, Enemy);
	}
}

bool AEnemyAIController::IsLowHealth() const
{
	if (const auto OwnedCharacter = GetPawn<ACBCharacter>())
		return OwnedCharacter->GetHealth() <= LowHealthThreshold;

	return false;
}

void AEnemyAIController::StartAttack()
{
	if (!IsValid(Blackboard)) return;

	SetFocus(TargetEnemy, EAIFocusPriority::Gameplay);

	if (const auto OwnerCharacter = GetPawn<AEnemyCharacter>())
	{
		// Get the equipped weapon
		AttackingWeapon = OwnerCharacter->GetEquippedItem<AWeaponItem>();

		if (IsValid(AttackingWeapon))
		{
			const int32 CurrentAttackers = Blackboard->GetValueAsInt(CurrentAttackersBlackboard);
			const int32 MaxConcurrentAttackers = Blackboard->GetValueAsInt(MaxAttackersBlackboard);
			// Check if can attack
			if (CurrentAttackers >= MaxConcurrentAttackers) return;
			// Incremeant current attackers
			Blackboard->SetValueAsInt(CurrentAttackersBlackboard, CurrentAttackers + 1);

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

	// Decremeant current attackers
	if (IsValid(Blackboard))
	{
		const int32 CurrentAttackers = Blackboard->GetValueAsInt(CurrentAttackersBlackboard);
		Blackboard->SetValueAsInt(CurrentAttackersBlackboard, (CurrentAttackers <= 0) ? 0 : CurrentAttackers - 1);
	}
}

bool AEnemyAIController::IsAttacking() const
{
	return (IsValid(AttackingWeapon) && AttackingWeapon->IsAttacking());
}