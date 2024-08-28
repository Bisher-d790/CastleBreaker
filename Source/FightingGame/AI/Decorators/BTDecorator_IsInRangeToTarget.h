#pragma once
#include "BehaviorTree/BTDecorator.h"

// Engine
#include "CoreMinimal.h"

#include "BTDecorator_IsInRangeToTarget.generated.h"

UCLASS()
class FIGHTINGGAME_API UBTDecorator_IsInRangeToTarget : public UBTDecorator
{
	GENERATED_BODY()

#pragma region Overrides
public:
	UBTDecorator_IsInRangeToTarget();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual FString GetStaticDescription() const override;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
#pragma endregion Overrides

#pragma region Blackboard
protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector OriginLocationKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard|Range")
	FBlackboardKeySelector MinRangeRadiusKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard|Range")
	FBlackboardKeySelector MaxRangeRadiusKey;
#pragma endregion Blackboard
};
