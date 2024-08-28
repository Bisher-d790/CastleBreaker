#include "BTTask_EnemyStartAttack.h"

// Engine

// Project
#include "FightingGame/AI/Enemy/EnemyAIController.h"


UBTTask_EnemyStartAttack::UBTTask_EnemyStartAttack(FObjectInitializer const& ObjectInitializer)
{
	bNotifyTick = false;

	NodeName = TEXT("Start Attack");
}

EBTNodeResult::Type UBTTask_EnemyStartAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto EnemyController = OwnerComp.GetAIOwner() ? Cast<AEnemyAIController>(OwnerComp.GetAIOwner()) : nullptr;
	if (!IsValid(EnemyController))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	EnemyController->StartAttack();

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

