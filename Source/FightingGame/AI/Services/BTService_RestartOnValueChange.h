#pragma once
#include "BehaviorTree/BTService.h"

// Engine
#include "CoreMinimal.h"

#include "BTService_RestartOnValueChange.generated.h"

/// <summary>
/// A Service that restarts the BehaviorTree if a specified Key Result Value has changed
/// </summary>
UCLASS()
class FIGHTINGGAME_API UBTService_RestartOnValueChange : public UBTService
{
	GENERATED_BODY()

#pragma region Overrides
public:
	UBTService_RestartOnValueChange();

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual FString GetStaticDescription() const override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
#pragma endregion Overrides

#pragma region Blackboard
protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector ValueToCheckKey;
#pragma endregion Blackboard

#pragma region Value
private:
	UPROPERTY() bool bOriginalValue = false;
#pragma endregion Value
};
