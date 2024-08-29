#pragma once
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"

// Engine
#include "CoreMinimal.h"

#include "BTTask_MoveToBlackboard.generated.h"

/// <summary>
/// An extending Task over the normal MoveTo task, where the Acceptable Radius could be set through a Blackboard Key
/// </summary>
UCLASS()
class FIGHTINGGAME_API UBTTask_MoveToBlackboard : public UBTTask_MoveTo
{
	GENERATED_BODY()

#pragma region Overrides
public:
	UBTTask_MoveToBlackboard(FObjectInitializer const& ObjectInitializer);

protected:
	virtual FString GetStaticDescription() const override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
#pragma endregion Overrides

#pragma region Blackboard
protected:
	// Overrides the Acceptable Radius value
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector AcceptableRadiusKey;
#pragma endregion Blackboard

};
