#include "BTService_RestartOnValueChange.h"

// Engine
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

// Project


UBTService_RestartOnValueChange::UBTService_RestartOnValueChange()
{
	// Don't share data across agents
	bCreateNodeInstance = true;

	NodeName = TEXT("Restart On Value Change");

	ValueToCheckKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_RestartOnValueChange, ValueToCheckKey), UObject::StaticClass());
	ValueToCheckKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_RestartOnValueChange, ValueToCheckKey));
}

void UBTService_RestartOnValueChange::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (const UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		ValueToCheckKey.ResolveSelectedKey(*BlackboardData);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize service: %s, make sure that Behavior Tree specifies Blackboard Asset!"), *GetName());
	}
}

FString UBTService_RestartOnValueChange::GetStaticDescription() const
{
	return Super::GetStaticDescription() +
		FString::Printf(TEXT("\nValue To Check Key: %s"),
			*ValueToCheckKey.SelectedKeyName.ToString());
}

void UBTService_RestartOnValueChange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (const auto BlackboardComponent = OwnerComp.GetBlackboardComponent())
	{
		const bool bValue = (ValueToCheckKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass()) ?
			BlackboardComponent->GetValueAsBool(ValueToCheckKey.SelectedKeyName) :
			IsValid(BlackboardComponent->GetValueAsObject(ValueToCheckKey.SelectedKeyName));

		// If value has changed, restart tree
		if (bValue != bOriginalValue)
		{
			OwnerComp.RestartTree();
			bOriginalValue = bValue;
		}
	}
}
