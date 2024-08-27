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
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UBTTask_FindPointInRangeToTarget(FObjectInitializer const& ObjectInitializer);

	virtual FString GetStaticDescription() const override;
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

#pragma region Functions
protected:
	FVector FindPointInRange(const FVector& OriginLocation, const FVector& DestinationLocation, const float MinRange,
		const float MaxRange) const;
#pragma endregion Functions
};
