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

	virtual FString GetStaticDescription() const override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
#pragma endregion Overrides

#pragma region Blackboard
protected:
	// The center of the circle in which the random point is searched
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector OriginLocationKey;

	// Variable in which the found random point is saved
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetLocationKey;
#pragma endregion Overrides

#pragma region Settings
protected:
	// Radius in which the random point has to be found
	UPROPERTY(EditAnywhere, Category = "Settings")
	float Radius = 1500.f;
#pragma endregion Settings
};
