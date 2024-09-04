#include "BTTask_MoveToBlackboard.h"

// Engine
#include "BehaviorTree/BlackboardComponent.h"

// Project


UBTTask_MoveToBlackboard::UBTTask_MoveToBlackboard(FObjectInitializer const& ObjectInitializer)
{
	AcceptableRadiusKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveToBlackboard, AcceptableRadiusKey));
}

FString UBTTask_MoveToBlackboard::GetStaticDescription() const
{
	return Super::GetStaticDescription() +
		FString::Printf(TEXT("\nAcceptable Radius Key: %s"),
			*AcceptableRadiusKey.SelectedKeyName.ToString());
}

void UBTTask_MoveToBlackboard::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		AcceptableRadiusKey.ResolveSelectedKey(*BlackboardData);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that Behavior Tree specifies Blackboard Asset!"), *GetName());
	}
}

EBTNodeResult::Type UBTTask_MoveToBlackboard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto BlackboardComponent = OwnerComp.GetBlackboardComponent())
	{
		AcceptableRadius = BlackboardComponent->GetValueAsFloat(AcceptableRadiusKey.SelectedKeyName);
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}