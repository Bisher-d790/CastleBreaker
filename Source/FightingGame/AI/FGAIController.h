#pragma once
#include "AIController.h"

// Engine
#include "CoreMinimal.h"

#include "FGAIController.generated.h"

class AEquippableItem;

UCLASS()
class FIGHTINGGAME_API AFGAIController : public AAIController
{
	GENERATED_BODY()

#pragma region Overrides
public:
	AFGAIController() = default;

protected:
	virtual void BeginPlay() override;
#pragma endregion Overrides

#pragma region EquippableItem
public:
	AEquippableItem* SpawnAndEquipNewItem(TSubclassOf<AEquippableItem> Item);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "EquippableItem")
	TSubclassOf<AEquippableItem> ItemToEquipOnBeginPlay = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EquippableItem")
	bool bEquipItemOnBeginPlay = true;
#pragma endregion EquippableItem
};
