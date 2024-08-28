#pragma once
#include "BehaviorTree/BTTaskNode.h"

// Engine
#include "CoreMinimal.h"

#include "BTTask_EnemyStartAttack.generated.h"

UCLASS()
class FIGHTINGGAME_API UBTTask_EnemyStartAttack : public UBTTaskNode
{
	GENERATED_BODY()

#pragma region Overrides
public:
	UBTTask_EnemyStartAttack(FObjectInitializer const& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
#pragma endregion Overrides
};
