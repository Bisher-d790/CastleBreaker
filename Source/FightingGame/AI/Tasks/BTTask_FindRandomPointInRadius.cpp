#include "BTTask_FindRandomPointInRadius.h"

// Engine
#include "AIController.h"
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
	RadiusKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomPointInRadius, RadiusKey));
}

FString UBTTask_FindRandomPointInRadius::GetStaticDescription() const
{
	return Super::GetStaticDescription() +
		FString::Printf(TEXT("\nOrigin Location: %s\nTarget Location: %s\nRadius: %s"),
			*OriginLocationKey.SelectedKeyName.ToString(),
			*TargetLocationKey.SelectedKeyName.ToString(),
			*RadiusKey.SelectedKeyName.ToString());
}

void UBTTask_FindRandomPointInRadius::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		OriginLocationKey.ResolveSelectedKey(*BlackboardData);
		TargetLocationKey.ResolveSelectedKey(*BlackboardData);
		RadiusKey.ResolveSelectedKey(*BlackboardData);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that Behavior Tree specifies Blackboard Asset!"), *GetName());
	}
}

EBTNodeResult::Type UBTTask_FindRandomPointInRadius::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto World = GetWorld();
	if (!IsValid(World))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	const auto BlackboardComponent = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetBlackboardComponent() : nullptr;
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

	const auto Radius = BlackboardComponent->GetValueAsFloat(RadiusKey.SelectedKeyName);

	// Get random location
	FNavLocation RandomLocation;
	if (!NavigationSystem->GetRandomPointInNavigableRadius(OriginLocation, Radius, RandomLocation, nullptr))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	// Set the random location
	BlackboardComponent->SetValueAsVector(TargetLocationKey.SelectedKeyName, RandomLocation.Location);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

