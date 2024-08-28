#include "BTTaskNode_SetFocus.h"

// Engine

// Project
#include "FightingGame/AI/Enemy/EnemyAIController.h"


UBTTaskNode_SetFocus::UBTTaskNode_SetFocus(FObjectInitializer const& ObjectInitializer)
{
	bNotifyTick = false;

	NodeName = TEXT("Set Focus");
}

EBTNodeResult::Type UBTTaskNode_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto EnemyController = OwnerComp.GetAIOwner() ? Cast<AEnemyAIController>(OwnerComp.GetAIOwner()) : nullptr;
	if (!IsValid(EnemyController))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	EnemyController->SetFocus(EnemyController->GetTargetEnemy());

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

