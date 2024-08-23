#include "WeaponItem.h"

// Engine
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


void AWeaponItem::StartPrimaryAction()
{
	Super::StartPrimaryAction();

	StartAttack();
}

void AWeaponItem::StartAttack()
{
	if (!CanAttack()) return;

	bIsAttacking = true;

	GetWorldTimerManager().SetTimer(AttackTimer, this, &AWeaponItem::FinishAttack, AttackDuration);

	// Disable movement
	if (const auto OwnerCharacter = GetOwner<ACharacter>())
	{
		if (const auto MovementComp = OwnerCharacter->GetCharacterMovement())
			MovementComp->DisableMovement();

		if (const auto OwnerMesh = OwnerCharacter->GetMesh())
			OwnerMesh->PlayAnimation(PrimaryActionAnimation, false);
	}
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
