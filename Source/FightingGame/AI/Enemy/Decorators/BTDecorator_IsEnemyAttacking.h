#pragma once
#include "BehaviorTree/BTDecorator.h"

// Engine
#include "CoreMinimal.h"

#include "BTDecorator_IsEnemyAttacking.generated.h"

UCLASS()
class FIGHTINGGAME_API UBTDecorator_IsEnemyAttacking : public UBTDecorator
{
	GENERATED_BODY()

#pragma region Overrides
public:
	UBTDecorator_IsEnemyAttacking();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
#pragma endregion Overrides
};
