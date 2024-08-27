#pragma once
#include "AIController.h"

// Engine
#include "CoreMinimal.h"

#include "FGAIController.generated.h"

UCLASS()
class FIGHTINGGAME_API AFGAIController : public AAIController
{
	GENERATED_BODY()

#pragma region Overrides
public:
	AFGAIController() = default;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
#pragma endregion Overrides

#pragma region AI
protected:
	UPROPERTY(EditAnywhere, Category = "AI|BehaviorTree")
	UBehaviorTree* BehaviorTree = nullptr;
#pragma endregion AI

#pragma region Death
protected:
	UFUNCTION() void HandleDeath();

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float DestroyTimeAfterDeath = 2.f;
#pragma region Death
};
