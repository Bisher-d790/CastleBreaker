#include "WeaponItem.h"

// Engine
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Project
#include "FightingGame/AI/Enemy/EnemyAIController.h"


void AWeaponItem::StartPrimaryAction()
{
	Super::StartPrimaryAction();

	StartAttack();
}

void AWeaponItem::StartAttack()
{
	if (!CanAttack()) return;

	bIsAttacking = true;

	// Disable movement
	if (const auto OwnerCharacter = GetOwner<ACharacter>())
	{
		if (const auto MovementComp = OwnerCharacter->GetCharacterMovement())
			MovementComp->DisableMovement();

		if (const auto OwnerMesh = OwnerCharacter->GetMesh())
			OwnerMesh->PlayAnimation(AttackAnimation, false);
	}

	// Finish with timer
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AWeaponItem::FinishAttack, AttackDuration);
}

void AWeaponItem::FinishAttack()
{
	bIsAttacking = false;

	// Enable movement
	if (const auto OwnerCharacter = GetOwner<ACharacter>())
	{
		if (const auto MovementComp = OwnerCharacter->GetCharacterMovement())
			MovementComp->SetMovementMode(EMovementMode::MOVE_Walking);

		if (const auto OwnerMesh = OwnerCharacter->GetMesh())
			OwnerMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}
}

bool AWeaponItem::CanAttack() const
{
	// Don't attack while in the air
	if (!bCanAttackInTheAir)
	{
		if (const auto OwnerCharacter = GetOwner<ACharacter>())
		{
			if (const auto MovementComp = OwnerCharacter->GetCharacterMovement())
				if (MovementComp->IsFalling()) return false;
		}
	}

	// Don't attack if is already attacking
	return !bIsAttacking;
}

bool AWeaponItem::CanDamageActor(const AActor* DamagedActor) const
{
	if (!IsValid(GetOwner())) return false;

	// If Owner is of Enemy type
	if (const auto OwnerController = GetOwner()->GetInstigatorController<AEnemyAIController>())
	{
		// Check if can only damage players, and if the victim is a player
		if(OwnerController->CanOnlyDamagePlayers())
			return IsValid(DamagedActor->GetInstigatorController<APlayerController>());
	}

	return true;
}
