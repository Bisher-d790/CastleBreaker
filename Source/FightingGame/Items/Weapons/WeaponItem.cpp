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
	if (bIsAttacking) return;

	bIsAttacking = true;

	GetWorldTimerManager().SetTimer(AttackTimer, this, &AWeaponItem::FinishAttack, AttackDuration);

	// Disable movement
	if (const auto OwnerCharacter = GetOwner<ACharacter>())
	{
		if (const auto MovementComp = OwnerCharacter->GetCharacterMovement())
			MovementComp->DisableMovement();
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
	}
}
