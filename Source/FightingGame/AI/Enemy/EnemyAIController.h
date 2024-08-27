#pragma once
#include "FightingGame/AI/FGAIController.h"

// Engine
#include "CoreMinimal.h"

#include "EnemyAIController.generated.h"


UCLASS()
class FIGHTINGGAME_API AEnemyAIController : public AFGAIController
{
	GENERATED_BODY()

#pragma region Settings
protected:
	virtual void SetupSettingsFromDT() override;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Settings|Blackboard")
	FName PatrolRadiusBlackboard = "PatrolRadius";
#pragma endregion Settings

#pragma region Enemy
public:
	void EnemyDetected(APawn* Enemy);

	UFUNCTION(BlueprintPure, Category = "AI|Enemy")
	FORCEINLINE APawn* GetTargetEnemy() const { return TargetEnemy; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDetected, APawn*, DetectedEnemy);
	FOnEnemyDetected OnEnemyDetected;

protected:
	void SetTargetEnemy(APawn* Enemy);

	UPROPERTY() APawn* TargetEnemy = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Enemy")
	FName TargetEnemyBlackboard = "TargetEnemy";
#pragma endregion Enemy
};
