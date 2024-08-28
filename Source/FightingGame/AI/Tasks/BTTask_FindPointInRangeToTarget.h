#pragma once
#include "BehaviorTree/BTTaskNode.h"

// Engine
#include "CoreMinimal.h"

#include "BTTask_FindPointInRangeToTarget.generated.h"

UCLASS()
class FIGHTINGGAME_API UBTTask_FindPointInRangeToTarget : public UBTTaskNode
{
	GENERATED_BODY()

#pragma region Overrides
public:
	UBTTask_FindPointInRangeToTarget(FObjectInitializer const& ObjectInitializer);

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
	FBlackboardKeySelector DestinationLocationKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard|Range")
	FBlackboardKeySelector MinRangeRadiusKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard|Range")
	FBlackboardKeySelector MaxRangeRadiusKey;
#pragma endregion Blackboard

#pragma region Navigation
protected:
	FVector FindPointInRange(const FVector& OriginLocation, const FVector& DestinationLocation, const float MinRange,
		const float MaxRange) const;
#pragma endregion Navigation

#pragma region Debug
protected:
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebugLines = false;

	UPROPERTY(EditAnywhere, Category = "Debug")
	float DrawDebugTime = 2.f;
#pragma endregion Debug
};
