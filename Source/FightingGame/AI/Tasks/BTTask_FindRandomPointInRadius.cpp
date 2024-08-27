#include "BTTask_FindRandomPointInRadius.h"

// Engine
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

// Project


UBTTask_FindRandomPointInRadius::UBTTask_FindRandomPointInRadius(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Random Point In Radius");

	OriginLocationKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomPointInRadius, OriginLocationKey), AActor::StaticClass());
	OriginLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomPointInRadius, OriginLocationKey));
	TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomPointInRadius, TargetLocationKey));
}

FString UBTTask_FindRandomPointInRadius::GetStaticDescription() const
{
	return Super::GetStaticDescription() +
		FString::Printf(TEXT("\nRadius: %0.1lf\nOrigin Location: %s\nTarget Location: %s"),
			Radius,
			*OriginLocationKey.SelectedKeyName.ToString(),
			*TargetLocationKey.SelectedKeyName.ToString());
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
	const auto NavSystem = UNavigationSystemV1::GetCurrent(World);
	FNavLocation RandomLocation;

	// Get Origin Location
	FVector OriginLocation = FVector::ZeroVector;
	// If key is object
	if (const auto OriginObject = BlackboardComponent->GetValueAsObject(OriginLocationKey.SelectedKeyName))
	{
		if (const auto OriginActor = Cast<AActor>(OriginObject))
			OriginLocation = OriginActor->GetActorLocation();
	}
	// If key is a vector
	else
	{
		OriginLocation = BlackboardComponent->GetValueAsVector(OriginLocationKey.SelectedKeyName);
	}

	// Null Checks, and get random location
	if (!IsValid(BlackboardComponent) || !IsValid(NavSystem) ||
		!NavSystem->GetRandomPointInNavigableRadius(OriginLocation, Radius, RandomLocation, nullptr))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	// Set the random location
	BlackboardComponent->SetValueAsVector(TargetLocationKey.SelectedKeyName, RandomLocation.Location);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

