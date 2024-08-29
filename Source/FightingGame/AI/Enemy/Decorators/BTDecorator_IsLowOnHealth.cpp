#include "BTDecorator_IsLowOnHealth.h"

// Engine

// Project
#include "FightingGame/AI/Enemy/EnemyAIController.h"


UBTDecorator_IsLowOnHealth::UBTDecorator_IsLowOnHealth()
{
	// Disable ticking
	bNotifyTick = false;
	bTickIntervals = false;

	NodeName = TEXT("Is Low On Health");
}

bool UBTDecorator_IsLowOnHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto EnemyController = OwnerComp.GetAIOwner() ? Cast<AEnemyAIController>(OwnerComp.GetAIOwner()) : nullptr;
	if (!IsValid(EnemyController)) return false;

	return EnemyController->IsLowHealth();
}

