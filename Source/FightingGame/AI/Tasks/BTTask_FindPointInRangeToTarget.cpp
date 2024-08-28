#include "BTTask_FindPointInRangeToTarget.h"

// Engine
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"

// Project


UBTTask_FindPointInRangeToTarget::UBTTask_FindPointInRangeToTarget(FObjectInitializer const& ObjectInitializer)
{
	bNotifyTick = false;

	NodeName = TEXT("Find Point In Range");

	OriginLocationKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPointInRangeToTarget, OriginLocationKey), AActor::StaticClass());
	OriginLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPointInRangeToTarget, OriginLocationKey));

	DestinationLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPointInRangeToTarget, DestinationLocationKey));
	DestinationLocationKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPointInRangeToTarget, DestinationLocationKey), AActor::StaticClass());

	TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPointInRangeToTarget, TargetLocationKey));

	MinRangeRadiusKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPointInRangeToTarget, MinRangeRadiusKey));
	MaxRangeRadiusKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPointInRangeToTarget, MaxRangeRadiusKey));
}

FString UBTTask_FindPointInRangeToTarget::GetStaticDescription() const
{
	return Super::GetStaticDescription() +
		FString::Printf(TEXT("\nOrigin Location: %s\nDestination Location: %s\nTarget Location: %s\nMin Range: %s\nMax Range: %s"),
			*OriginLocationKey.SelectedKeyName.ToString(),
			*DestinationLocationKey.SelectedKeyName.ToString(),
			*TargetLocationKey.SelectedKeyName.ToString(),
			*MinRangeRadiusKey.SelectedKeyName.ToString(),
			*MaxRangeRadiusKey.SelectedKeyName.ToString());
}

void UBTTask_FindPointInRangeToTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		OriginLocationKey.ResolveSelectedKey(*BlackboardData);
		DestinationLocationKey.ResolveSelectedKey(*BlackboardData);
		TargetLocationKey.ResolveSelectedKey(*BlackboardData);
		MinRangeRadiusKey.ResolveSelectedKey(*BlackboardData);
		MaxRangeRadiusKey.ResolveSelectedKey(*BlackboardData);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that Behavior Tree specifies Blackboard Asset!"), *GetName());
	}
}

EBTNodeResult::Type UBTTask_FindPointInRangeToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto World = GetWorld();
	if (!IsValid(World))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	const auto NavigationSystem = UNavigationSystemV1::GetCurrent(World);
	if (!IsValid(BlackboardComponent) || !IsValid(NavigationSystem))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	// Get Origin Location
	FVector OriginLocation = FVector::ZeroVector;
	// If key is an object
	if (const auto OriginObject = BlackboardComponent->GetValueAsObject(OriginLocationKey.SelectedKeyName))
	{
		if (const auto OriginActor = Cast<AActor>(OriginObject))
			OriginLocation = OriginActor->GetActorLocation();
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return EBTNodeResult::Failed;
		}
	}
	// If key is a vector
	else
	{
		OriginLocation = BlackboardComponent->GetValueAsVector(OriginLocationKey.SelectedKeyName);
	}

	// Get Destination Location
	FVector DestinationLocation = FVector::ZeroVector;
	// If key is an object
	if (const auto DestinationObject = BlackboardComponent->GetValueAsObject(DestinationLocationKey.SelectedKeyName))
	{
		if (const auto DestinationActor = Cast<AActor>(DestinationObject))
			DestinationLocation = DestinationActor->GetActorLocation();
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return EBTNodeResult::Failed;
		}
	}
	// If key is a vector
	else
	{
		DestinationLocation = BlackboardComponent->GetValueAsVector(DestinationLocationKey.SelectedKeyName);
	}

	// Get Min and Max Range
	const float MinRange = BlackboardComponent->GetValueAsFloat(MinRangeRadiusKey.SelectedKeyName);
	const float MaxRange = BlackboardComponent->GetValueAsFloat(MaxRangeRadiusKey.SelectedKeyName);

	// Calculate Target Location in range from the Origin towards the Destination
	FNavLocation TargetLocation = FNavLocation(FindPointInRange(OriginLocation, DestinationLocation, MinRange, MaxRange));

	// Project the point to the navigation mesh
	NavigationSystem->ProjectPointToNavigation(TargetLocation, TargetLocation);

	// Set the target location
	BlackboardComponent->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetLocation);

	if (bDrawDebugLines)
	{
		DrawDebugSphere(World, DestinationLocation, MinRange, 12, FColor::Red, false, DrawDebugTime);
		DrawDebugSphere(World, DestinationLocation, MaxRange, 12, FColor::Blue, false, DrawDebugTime);
		DrawDebugSphere(World, TargetLocation, 100.f, 12, FColor::Yellow, false, DrawDebugTime);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FVector UBTTask_FindPointInRangeToTarget::FindPointInRange(const FVector& OriginLocation, const FVector& DestinationLocation,
	const float MinRange, const float MaxRange) const
{
	// Init target location
	FVector TargetLocation = OriginLocation;
	const float DistanceToDestination = FVector::Distance(OriginLocation, DestinationLocation);

	// Distance between the Origin and the start of the range
	float DistanceToTarget = 0.f;

	// Get the normal directional vector pointing from the Origin to the Destination
	TargetLocation = (DestinationLocation - OriginLocation).GetSafeNormal();

	// If closer, point backwards
	if (DistanceToDestination <= MinRange)
	{
		TargetLocation.X *= -1;
		TargetLocation.Y *= -1;

		DistanceToTarget = MinRange - DistanceToDestination;
	}
	// Else, point towards the start of the range
	else if (DistanceToDestination >= MaxRange)
	{
		DistanceToTarget = DistanceToDestination - MaxRange;
	}

	// Add the distance into the middle of the range
	DistanceToTarget += (MaxRange - MinRange) / 2;

	// Multiply by the distance between Origin and Destination to get the actual point inside the range relative to the Origin location
	TargetLocation *= DistanceToTarget;

	// Add the Origin Location to get the World Location
	TargetLocation += OriginLocation;

	return TargetLocation;
}
