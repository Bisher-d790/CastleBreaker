#include "FGAIController.h"

// Project
#include "FightingGame/Character/FGCharacter.h"
#include "FightingGame/Items/EquippableItem.h"


void AFGAIController::BeginPlay()
{
	Super::BeginPlay();

	if (bEquipItemOnBeginPlay)
		SpawnAndEquipNewItem(ItemToEquipOnBeginPlay);
}

AEquippableItem* AFGAIController::SpawnAndEquipNewItem(TSubclassOf<AEquippableItem> Item)
{
	const auto World = GetWorld();
	if (!IsValid(Item) || !IsValid(World)) return nullptr;

	if (const auto SpawnedItem = World->SpawnActor<AEquippableItem>(Item))
	{
		if (auto OwnedPawn = GetPawn<AFGCharacter>())
			OwnedPawn->EquipItem(SpawnedItem);

		return SpawnedItem;
	}

	return nullptr;
}