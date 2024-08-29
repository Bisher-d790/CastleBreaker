#include "BTDecorator_CompareBBKeys.h"

// Engine
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"

// Project


UBTDecorator_CompareBBKeys::UBTDecorator_CompareBBKeys()
{
	bNotifyTick = true;
	bTickIntervals = true;

	NodeName = TEXT("Compare Blackboard Keys");

	FirstBlackboardKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_CompareBBKeys, FirstBlackboardKey));
	SecondBlackboardKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_CompareBBKeys, SecondBlackboardKey));

	FirstBlackboardKey.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_CompareBBKeys, FirstBlackboardKey));
	SecondBlackboardKey.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_CompareBBKeys, SecondBlackboardKey));

	FirstBlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_CompareBBKeys, FirstBlackboardKey));
	SecondBlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_CompareBBKeys, SecondBlackboardKey));

	FirstBlackboardKey.AddRotatorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_CompareBBKeys, FirstBlackboardKey));
	SecondBlackboardKey.AddRotatorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_CompareBBKeys, SecondBlackboardKey));
}

void UBTDecorator_CompareBBKeys::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		FirstBlackboardKey.ResolveSelectedKey(*BlackboardData);
		SecondBlackboardKey.ResolveSelectedKey(*BlackboardData);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize decorator: %s, make sure that Behavior Tree specifies Blackboard Asset!"), *GetName());
	}
}

FString UBTDecorator_CompareBBKeys::GetStaticDescription() const
{
	return Super::GetStaticDescription() +
		FString::Printf(TEXT("\nFirst Blackboard Key: %s\nSecond Blackboard Key: %s"),
			*FirstBlackboardKey.SelectedKeyName.ToString(),
			*SecondBlackboardKey.SelectedKeyName.ToString());
}

bool UBTDecorator_CompareBBKeys::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComponent)) return false;

	if (FirstBlackboardKey.SelectedKeyType != SecondBlackboardKey.SelectedKeyType) return false;

	float Value1 = 0.f, Value2 = 0.f;

	if (FirstBlackboardKey.SelectedKeyType == UBlackboardKeyType_Float::StaticClass())
	{
		Value1 = BlackboardComponent->GetValueAsFloat(FirstBlackboardKey.SelectedKeyName);
		Value2 = BlackboardComponent->GetValueAsFloat(SecondBlackboardKey.SelectedKeyName);
	}
	else if (FirstBlackboardKey.SelectedKeyType == UBlackboardKeyType_Int::StaticClass())
	{
		Value1 = (float)BlackboardComponent->GetValueAsInt(FirstBlackboardKey.SelectedKeyName);
		Value2 = (float)BlackboardComponent->GetValueAsInt(SecondBlackboardKey.SelectedKeyName);
	}
	else if (FirstBlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		Value1 = BlackboardComponent->GetValueAsVector(FirstBlackboardKey.SelectedKeyName).Size();
		Value2 = BlackboardComponent->GetValueAsVector(SecondBlackboardKey.SelectedKeyName).Size();
	}
	else if (FirstBlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
	{
		Value1 = BlackboardComponent->GetValueAsRotator(FirstBlackboardKey.SelectedKeyName).Vector().Size();
		Value2 = BlackboardComponent->GetValueAsRotator(SecondBlackboardKey.SelectedKeyName).Vector().Size();
	}

	switch (ComparisonType)
	{
	case EBBComparisonType::Equals:
		return Value1 == Value2;
		break;
	case EBBComparisonType::NotEquals:
		return Value1 != Value2;
		break;
	case EBBComparisonType::Bigger:
		return Value1 > Value2;
		break;
	case EBBComparisonType::BiggerOrEquals:
		return Value1 >= Value2;
		break;
	case EBBComparisonType::Smaller:
		return Value1 < Value2;
		break;
	case EBBComparisonType::SmallerOrEquals:
		return Value1 <= Value2;
		break;
	}

	return false;
}
