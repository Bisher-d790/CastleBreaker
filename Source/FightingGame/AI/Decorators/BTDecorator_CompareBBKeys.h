#pragma once
#include "BehaviorTree/BTDecorator.h"

// Engine
#include "CoreMinimal.h"

#include "BTDecorator_CompareBBKeys.generated.h"

UENUM(BlueprintType, Blueprintable)
enum class EBBComparisonType : uint8
{
	Equals,
	NotEquals,
	Bigger,
	BiggerOrEquals,
	Smaller,
	SmallerOrEquals
};

UCLASS()
class FIGHTINGGAME_API UBTDecorator_CompareBBKeys : public UBTDecorator
{
	GENERATED_BODY()

#pragma region Overrides
public:
	UBTDecorator_CompareBBKeys();

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual FString GetStaticDescription() const override;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
#pragma endregion Overrides

#pragma region Blackboard
protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector FirstBlackboardKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector SecondBlackboardKey;
#pragma endregion Blackboard

#pragma region Settings
protected:
	UPROPERTY(EditAnywhere, Category = Settings)
	EBBComparisonType ComparisonType = EBBComparisonType::Equals;
#pragma endregion Settings
};
