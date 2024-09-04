#pragma once
#include "BehaviorTree/BTTaskNode.h"

//Engine
#include "CoreMinimal.h"

#include "BTTask_FindRandomPointInRadius.generated.h"

/// <summary>
/// A simple Task to get a Random Point with in a Min and Max Range Radius, relative to an OriginLocation
/// </summary>
UCLASS()
class CASTLEBREAKER_API UBTTask_FindRandomPointInRadius : public UBTTaskNode
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
	// Center point to get the Random location around
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector OriginLocationKey;

	// Target Location where to store the value
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetLocationKey;

	// Min Radius range, min distance to Origin
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector MinRadiusKey;

	// Max Radius range, max distance to Origin
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector MaxRadiusKey;
#pragma endregion Overrides

#pragma region Navigation
protected:
	bool GetRandomLocationInRange(const FVector& CenterLocation, const float MinRadius, const float MaxRadius, FNavLocation& OutLocation);
#pragma endregion Navigation
};
