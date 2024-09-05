#include "CBAIController.h"

// Engine
#include "BehaviorTree/BehaviorTree.h"

// Project
#include "CastleBreaker/AI/CBAICharacter.h"
#include "CastleBreaker/Items/EquippableItem.h"
#include "CastleBreaker/Components/HealthComponent.h"


void ACBAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (const auto OwnedCharacter = Cast<ACBAICharacter>(InPawn))
	{
		if (const auto HealthComponent = OwnedCharacter->GetHealthComponent())
			if (!HealthComponent->OnDeath.IsAlreadyBound(this, &ThisClass::HandleDeath))
				HealthComponent->OnDeath.AddDynamic(this, &ThisClass::HandleDeath);
	}

	if (bStartAILogicOnPossess && IsValid(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);

		SetupSettingsFromDT();
	}
}

void ACBAIController::OnUnPossess()
{
	if (const auto OwnedCharacter = GetPawn<ACBAICharacter>())
	{
		if (const auto HealthComponent = OwnedCharacter->GetHealthComponent())
			if (HealthComponent->OnDeath.IsAlreadyBound(this, &ThisClass::HandleDeath))
				HealthComponent->OnDeath.RemoveDynamic(this, &ThisClass::HandleDeath);
	}

	Super::OnUnPossess();
}

void ACBAIController::HandleDeath()
{
	if (const auto OwnedCharacter = GetPawn<ACBAICharacter>())
	{
		if (const auto CharacterMesh = OwnedCharacter->GetMesh())
			CharacterMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

		OwnedCharacter->UnEquipItem();
		OwnedCharacter->SetLifeSpan(DestroyTimeAfterDeath);
	}

	SetLifeSpan(DestroyTimeAfterDeath);
}
