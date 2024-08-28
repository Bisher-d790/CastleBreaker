#include "BTDecorator_IsEnemyAttacking.h"

// Engine

// Project
#include "FightingGame/AI/Enemy/EnemyAIController.h"


UBTDecorator_IsEnemyAttacking::UBTDecorator_IsEnemyAttacking()
{
	// Disable ticking
	bNotifyTick = false;
	bTickIntervals = false;

	NodeName = TEXT("Is Attacking");
}

bool UBTDecorator_IsEnemyAttacking::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto EnemyController = OwnerComp.GetAIOwner() ? Cast<AEnemyAIController>(OwnerComp.GetAIOwner()) : nullptr;
	if (!IsValid(EnemyController)) return false;

	return EnemyController->IsAttacking();
}
