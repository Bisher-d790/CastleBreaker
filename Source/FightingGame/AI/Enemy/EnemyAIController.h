#pragma once
#include "FightingGame/AI/FGAIController.h"

// Engine
#include "CoreMinimal.h"

#include "EnemyAIController.generated.h"

class AWeaponItem;

UCLASS()
class FIGHTINGGAME_API AEnemyAIController : public AFGAIController
{
	GENERATED_BODY()

#pragma region Overrides
protected:
	virtual void Tick(float DeltaTime) override;
#pragma endregion Overrides

#pragma region Settings
protected:
	virtual void SetupSettingsFromDT() override;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Settings|Blackboard")
	FName MinPatrolRadiusBlackboard = "MinPatrolRadius";
	UPROPERTY(EditDefaultsOnly, Category = "AI|Settings|Blackboard")
	FName MaxPatrolRadiusBlackboard = "MaxPatrolRadius";
	UPROPERTY(EditDefaultsOnly, Category = "AI|Settings|Blackboard")
	FName MinCombatRangeBlackboard = "MinCombatRange";
	UPROPERTY(EditDefaultsOnly, Category = "AI|Settings|Blackboard")
	FName MaxCombatRangeBlackboard = "MaxCombatRange";
	UPROPERTY(EditDefaultsOnly, Category = "AI|Settings|Blackboard")
	FName AttackDistanceBlackboard = "AttackDistance";
	UPROPERTY(EditDefaultsOnly, Category = "AI|Settings|Blackboard")
	FName MaxAttackersBlackboard = "MaxConcurrentAttackers";
#pragma endregion Settings

#pragma region Enemy
public:
	UFUNCTION()
	virtual void EnemyDetected(APawn* Enemy);

	UFUNCTION()
	virtual void EnemyUnDetected();

	UFUNCTION(BlueprintPure, Category = "AI|Enemy")
	FORCEINLINE APawn* GetTargetEnemy() const { return TargetEnemy; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDetected, APawn*, DetectedEnemy);
	FOnEnemyDetected OnEnemyDetected;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyUnDetected);
	FOnEnemyUnDetected OnEnemyUnDetected;

protected:
	void SetTargetEnemy(APawn* Enemy);

	UPROPERTY() APawn* TargetEnemy = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Enemy")
	FName TargetEnemyBlackboard = "TargetEnemy";
#pragma endregion Enemy

#pragma region Combat
public:
	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void StartAttack();

	UFUNCTION(BlueprintPure, Category = "AI|Combat")
	bool IsAttacking() const;

protected:
	void FinishAttack();

	UPROPERTY() AWeaponItem* AttackingWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Combat")
	FName CurrentAttackersBlackboard = "CurrentAttackers";
#pragma endregion Combat
};
