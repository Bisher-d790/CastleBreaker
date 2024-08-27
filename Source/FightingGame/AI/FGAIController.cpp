#include "FGAIController.h"

// Engine
#include "AIModule/Classes/BehaviorTree/BehaviorTree.h"

// Project
#include "FightingGame/AI/FGAICharacter.h"
#include "FightingGame/Items/EquippableItem.h"
#include "FightingGame/Components/HealthComponent.h"


void AFGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (const auto OwnedCharacter = Cast<AFGAICharacter>(InPawn))
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

void AFGAIController::OnUnPossess()
{
	if (const auto OwnedCharacter = GetPawn<AFGAICharacter>())
	{
		if (const auto HealthComponent = OwnedCharacter->GetHealthComponent())
			if (HealthComponent->OnDeath.IsAlreadyBound(this, &ThisClass::HandleDeath))
				HealthComponent->OnDeath.RemoveDynamic(this, &ThisClass::HandleDeath);
	}

	Super::OnUnPossess();
}

void AFGAIController::HandleDeath()
{
	if (const auto OwnedCharacter = GetPawn<AFGAICharacter>())
	{
		if (const auto CharacterMesh = OwnedCharacter->GetMesh())
			CharacterMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

		OwnedCharacter->UnEquipItem();
		OwnedCharacter->SetLifeSpan(DestroyTimeAfterDeath);
	}

	SetLifeSpan(DestroyTimeAfterDeath);
}
