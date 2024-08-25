#include "FGAIController.h"

// Engine
#include "AIModule/Classes/BehaviorTree/BehaviorTree.h"

// Project
#include "FightingGame/Character/FGCharacter.h"
#include "FightingGame/Items/EquippableItem.h"
#include "FightingGame/Components/HealthComponent.h"


void AFGAIController::BeginPlay()
{
	Super::BeginPlay();

	if (bEquipItemOnBeginPlay)
		SpawnAndEquipNewItem(ItemToEquipOnBeginPlay);

	if (auto OwnedCharacter = GetPawn<AFGCharacter>())
	{
		if (const auto HealthComponent = OwnedCharacter->GetHealthComponent())
			if (!HealthComponent->OnDeath.IsAlreadyBound(this, &ThisClass::HandleDeath))
				HealthComponent->OnDeath.AddDynamic(this, &ThisClass::HandleDeath);
	}
}

void AFGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (const auto OwnedCharacter = Cast<AFGCharacter>(InPawn))
	{
		if (const auto HealthComponent = OwnedCharacter->GetHealthComponent())
			if (HealthComponent->OnDeath.IsAlreadyBound(this, &ThisClass::HandleDeath))
				HealthComponent->OnDeath.RemoveDynamic(this, &ThisClass::HandleDeath);
	}

	if (bStartAILogicOnPossess && IsValid(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);
	}
}

void AFGAIController::OnUnPossess()
{
	if (const auto OwnedCharacter = GetPawn<AFGCharacter>())
	{
		if (const auto HealthComponent = OwnedCharacter->GetHealthComponent())
			if (HealthComponent->OnDeath.IsAlreadyBound(this, &ThisClass::HandleDeath))
				HealthComponent->OnDeath.RemoveDynamic(this, &ThisClass::HandleDeath);
	}

	Super::OnUnPossess();
}

AEquippableItem* AFGAIController::SpawnAndEquipNewItem(TSubclassOf<AEquippableItem> Item)
{
	const auto World = GetWorld();
	if (!IsValid(Item) || !IsValid(World)) return nullptr;

	if (const auto SpawnedItem = World->SpawnActor<AEquippableItem>(Item))
	{
		if (const auto OwnedCharacter = GetPawn<AFGCharacter>())
			OwnedCharacter->EquipItem(SpawnedItem);

		return SpawnedItem;
	}

	return nullptr;
}

void AFGAIController::HandleDeath()
{
	if (const auto OwnedCharacter = GetPawn<AFGCharacter>())
	{
		if (const auto CharacterMesh = OwnedCharacter->GetMesh())
			CharacterMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

		OwnedCharacter->UnEquipItem();
		OwnedCharacter->SetLifeSpan(DestroyTimeAfterDeath);
	}

	SetLifeSpan(DestroyTimeAfterDeath);
}
