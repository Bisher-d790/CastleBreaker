#pragma once
#include "BehaviorTree/BTTaskNode.h"

// Engine
#include "CoreMinimal.h"

#include "BTTaskNode_SetFocus.generated.h"

UCLASS()
class FIGHTINGGAME_API UBTTaskNode_SetFocus : public UBTTaskNode
{
	GENERATED_BODY()

#pragma region Overrides
public:
	UBTTaskNode_SetFocus(FObjectInitializer const& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
#pragma endregion Overrides
};
