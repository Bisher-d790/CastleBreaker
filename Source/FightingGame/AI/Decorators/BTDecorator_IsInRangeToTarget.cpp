#include "BTDecorator_IsInRangeToTarget.h"

// Engine
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Project


UBTDecorator_IsInRangeToTarget::UBTDecorator_IsInRangeToTarget()
{
	// Disable ticking
	bNotifyTick = false;
	bTickIntervals = false;

	NodeName = TEXT("Is In Range");

	OriginLocationKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsInRangeToTarget, OriginLocationKey), AActor::StaticClass());
	OriginLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsInRangeToTarget, OriginLocationKey));

	TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsInRangeToTarget, TargetLocationKey));
	TargetLocationKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsInRangeToTarget, TargetLocationKey), AActor::StaticClass());

	MinRangeRadiusKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsInRangeToTarget, MinRangeRadiusKey));
	MaxRangeRadiusKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsInRangeToTarget, MaxRangeRadiusKey));
}

void UBTDecorator_IsInRangeToTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		OriginLocationKey.ResolveSelectedKey(*BlackboardData);
		TargetLocationKey.ResolveSelectedKey(*BlackboardData);
		MinRangeRadiusKey.ResolveSelectedKey(*BlackboardData);
		MaxRangeRadiusKey.ResolveSelectedKey(*BlackboardData);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize decorator: %s, make sure that Behavior Tree specifies Blackboard Asset!"), *GetName());
	}
}

FString UBTDecorator_IsInRangeToTarget::GetStaticDescription() const
{
	return Super::GetStaticDescription() +
		FString::Printf(TEXT("\nOrigin Key: %s\nTarget Key: %s\nMin Range: %s\nMax Range: %s"),
			*OriginLocationKey.SelectedKeyName.ToString(),
			*TargetLocationKey.SelectedKeyName.ToString(),
			*MinRangeRadiusKey.SelectedKeyName.ToString(),
			*MaxRangeRadiusKey.SelectedKeyName.ToString());
}

bool UBTDecorator_IsInRangeToTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto World = GetWorld();
	if (!IsValid(World)) return false;

	const auto BlackboardComponent = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetBlackboardComponent() : nullptr;
	if (!IsValid(BlackboardComponent)) return false;

	// Get Origin Location
	FVector OriginLocation = FVector::ZeroVector;
	// If key is an object
	if (const auto OriginObject = BlackboardComponent->GetValueAsObject(OriginLocationKey.SelectedKeyName))
	{
		if (const auto OriginActor = Cast<AActor>(OriginObject))
			OriginLocation = OriginActor->GetActorLocation();
		else return false;
	}
	// If key is a vector
	else
	{
		OriginLocation = BlackboardComponent->GetValueAsVector(OriginLocationKey.SelectedKeyName);
	}

	// Get Destination Location
	FVector TargetLocation = FVector::ZeroVector;
	// If key is an object
	if (const auto TargetObject = BlackboardComponent->GetValueAsObject(TargetLocationKey.SelectedKeyName))
	{
		if (const auto TargetActor = Cast<AActor>(TargetObject))
			TargetLocation = TargetActor->GetActorLocation();
		else return false;
	}
	// If key is a vector
	else
	{
		TargetLocation = BlackboardComponent->GetValueAsVector(TargetLocationKey.SelectedKeyName);
	}

	// Get Min and Max Range
	const float MinRange = BlackboardComponent->GetValueAsFloat(MinRangeRadiusKey.SelectedKeyName);
	const float MaxRange = BlackboardComponent->GetValueAsFloat(MaxRangeRadiusKey.SelectedKeyName);

	// check if is in combat range
	const float DistanceToTarget = FVector::Distance(OriginLocation, TargetLocation);

	// Set bool value to blackboard key
	return (DistanceToTarget >= MinRange && DistanceToTarget <= MaxRange);
}
