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

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
#pragma endregion Overrides

#pragma region AI
protected:
	UPROPERTY(EditAnywhere, Category = "AI|BehaviorTree")
	UBehaviorTree* BehaviorTree = nullptr;
#pragma endregion AI

#pragma region EquippableItem
public:
	AEquippableItem* SpawnAndEquipNewItem(TSubclassOf<AEquippableItem> Item);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "EquippableItem")
	TSubclassOf<AEquippableItem> ItemToEquipOnBeginPlay = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EquippableItem")
	bool bEquipItemOnBeginPlay = true;
#pragma endregion EquippableItem

#pragma region Death
protected:
	UFUNCTION() void HandleDeath();

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float DestroyTimeAfterDeath = 2.f;
#pragma region Death
};
