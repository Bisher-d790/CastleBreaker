#pragma once
#include "BehaviorTree/BTDecorator.h"

// Engine
#include "CoreMinimal.h"

#include "BTDecorator_IsLowOnHealth.generated.h"

/// <summary>
/// Decorator specific to an AEnemyAIController, to check whether it's low on health
/// </summary>
UCLASS()
class CASTLEBREAKER_API UBTDecorator_IsLowOnHealth : public UBTDecorator
{
	GENERATED_BODY()

#pragma region Overrides
public:
	UBTDecorator_IsLowOnHealth();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
#pragma endregion Overrides
};
