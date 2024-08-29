#pragma once
#include "BehaviorTree/BTDecorator.h"

// Engine
#include "CoreMinimal.h"

#include "BTDecorator_IsLowOnHealth.generated.h"

UCLASS()
class FIGHTINGGAME_API UBTDecorator_IsLowOnHealth : public UBTDecorator
{
	GENERATED_BODY()

#pragma region Overrides
public:
	UBTDecorator_IsLowOnHealth();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
#pragma endregion Overrides
};
