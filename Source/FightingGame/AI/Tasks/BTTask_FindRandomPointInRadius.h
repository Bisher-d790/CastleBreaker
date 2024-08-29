#pragma once
#include "BehaviorTree/BTTaskNode.h"

//Engine
#include "CoreMinimal.h"

#include "BTTask_FindRandomPointInRadius.generated.h"


UCLASS()
class FIGHTINGGAME_API UBTTask_FindRandomPointInRadius : public UBTTaskNode
{
	GENERATED_BODY()

#pragma region Overrides
public:
	UBTTask_FindRandomPointInRadius(FObjectInitializer const& ObjectInitializer);

protected:
	virtual FString GetStaticDescription() const override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
#pragma endregion Overrides

#pragma region Blackboard
protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector OriginLocationKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector MinRadiusKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector MaxRadiusKey;
#pragma endregion Overrides

#pragma region Navigation
protected:
	bool GetRandomLocationInRange(const FVector& CenterLocation, const float MinRadius, const float MaxRadius, FNavLocation& OutLocation);
#pragma endregion Navigation
};
