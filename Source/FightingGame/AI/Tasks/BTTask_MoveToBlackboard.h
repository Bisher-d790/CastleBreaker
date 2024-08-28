#pragma once
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"

// Engine
#include "CoreMinimal.h"

#include "BTTask_MoveToBlackboard.generated.h"

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
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector AcceptableRadiusKey;
#pragma endregion Blackboard

};
