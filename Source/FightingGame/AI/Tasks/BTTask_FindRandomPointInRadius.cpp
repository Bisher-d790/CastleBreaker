#include "BTTask_FindRandomPointInRadius.h"

// Engine
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

// Project


UBTTask_FindRandomPointInRadius::UBTTask_FindRandomPointInRadius(FObjectInitializer const& ObjectInitializer)
{
	bNotifyTick = false;

	NodeName = TEXT("Find Random Point In Radius");

	OriginLocationKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomPointInRadius, OriginLocationKey), AActor::StaticClass());
	OriginLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomPointInRadius, OriginLocationKey));
	TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomPointInRadius, TargetLocationKey));
	MinRadiusKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomPointInRadius, MinRadiusKey));
	MaxRadiusKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomPointInRadius, MaxRadiusKey));
}

FString UBTTask_FindRandomPointInRadius::GetStaticDescription() const
{
	return Super::GetStaticDescription() +
		FString::Printf(TEXT("\nOrigin Location: %s\nTarget Location: %s\nMin Radius: %s\nMax Radius: %s"),
			*OriginLocationKey.SelectedKeyName.ToString(),
			*TargetLocationKey.SelectedKeyName.ToString(),
			*MinRadiusKey.SelectedKeyName.ToString(),
			*MaxRadiusKey.SelectedKeyName.ToString());
}

void UBTTask_FindRandomPointInRadius::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		OriginLocationKey.ResolveSelectedKey(*BlackboardData);
		TargetLocationKey.ResolveSelectedKey(*BlackboardData);
		MinRadiusKey.ResolveSelectedKey(*BlackboardData);
		MaxRadiusKey.ResolveSelectedKey(*BlackboardData);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that Behavior Tree specifies Blackboard Asset!"), *GetName());
	}
}

EBTNodeResult::Type UBTTask_FindRandomPointInRadius::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComponent))
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

	const auto MinRadius = BlackboardComponent->GetValueAsFloat(MinRadiusKey.SelectedKeyName);
	const auto MaxRadius = BlackboardComponent->GetValueAsFloat(MaxRadiusKey.SelectedKeyName);

	// Get random location
	FNavLocation RandomLocation;
	if (!GetRandomLocationInRange(OriginLocation, MinRadius, MaxRadius, RandomLocation))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	// Set the random location
	BlackboardComponent->SetValueAsVector(TargetLocationKey.SelectedKeyName, RandomLocation.Location);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UBTTask_FindRandomPointInRadius::GetRandomLocationInRange(const FVector& CenterLocation, const float MinRadius, const float MaxRadius, FNavLocation& OutLocation)
{
	const auto NavigationSystem = GetWorld() ? UNavigationSystemV1::GetNavigationSystem(GetWorld()) : nullptr;
	if (!NavigationSystem) return false;

	// Get a random Radius within range
	const float Radius = FMath::RandRange(MinRadius, MaxRadius);

	// Get a random point within radius
	const FVector& RandomPoint = NavigationSystem->GetRandomPointInNavigableRadius(GetWorld(), CenterLocation, Radius);

	// Make sure the point is exactly at the Radius
	const FVector& TargetLocation = CenterLocation + ((RandomPoint - CenterLocation).GetSafeNormal() * Radius);

	return NavigationSystem->ProjectPointToNavigation(TargetLocation, OutLocation);
}

