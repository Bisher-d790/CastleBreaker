#pragma once
#include "BehaviorTree/BTDecorator.h"

// Engine
#include "CoreMinimal.h"

#include "BTDecorator_CompareBBKeys.generated.h"

/*
* The types of comparison possible
*/
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

/// <summary>
/// Decorator to compare between to Blackboard Key values
/// </summary>
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
	// First hand of the comparison, should be the same type as the SecondKey
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector FirstBlackboardKey;

	// Second hand of the comparison, should be the same type as the FirstKey
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector SecondBlackboardKey;
#pragma endregion Blackboard

#pragma region Settings
protected:
	UPROPERTY(EditAnywhere, Category = Settings)
	EBBComparisonType ComparisonType = EBBComparisonType::Equals;
#pragma endregion Settings
};
