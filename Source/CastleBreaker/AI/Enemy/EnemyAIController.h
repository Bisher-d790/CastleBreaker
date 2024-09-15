#pragma once
#include "CastleBreaker/AI/CBAIController.h"

// Engine
#include "CoreMinimal.h"

#include "EnemyAIController.generated.h"

class AWeaponItem;

/// <summary>
/// The main controller for the Enemy type, it acts as an Enemy AI to the player and attacks him
/// </summary>
UCLASS()
class CASTLEBREAKER_API AEnemyAIController : public ACBAIController
{
	GENERATED_BODY()

#pragma region Overrides
protected:
	virtual void Tick(float DeltaTime) override;
#pragma endregion Overrides

#pragma region Settings
protected:
	// Setup the values from a Settings DT
	virtual void SetupSettingsFromDT() override;

	// Blackboard Keys Names to be set from the Settings
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

	UPROPERTY() TObjectPtr<APawn> TargetEnemy = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Enemy")
	FName TargetEnemyBlackboard = "TargetEnemy";
#pragma endregion Enemy

#pragma region Health
public:
	UFUNCTION(BlueprintPure, Category = "AI|Health")
	FORCEINLINE float GetLowHealthThreshold() const { return LowHealthThreshold; }

	UFUNCTION(BlueprintPure, Category = "AI|Health")
	bool IsLowHealth() const;

protected:
	// Set from the Settings DT
	UPROPERTY() float LowHealthThreshold = 0.f;
#pragma endregion Health

#pragma region Combat
public:
	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void StartAttack();

	UFUNCTION(BlueprintPure, Category = "AI|Combat")
	bool IsAttacking() const;

	UFUNCTION(BlueprintPure, Category = "AI|Combat")
	FORCEINLINE bool CanOnlyDamagePlayers() const { return bOnlyDamagePlayers; };

protected:
	void FinishAttack();

	UPROPERTY() TObjectPtr<AWeaponItem> AttackingWeapon = nullptr;

	// Blackboard Key to be set when attacking, keeps track of the number of enemies currently attacking
	UPROPERTY(EditDefaultsOnly, Category = "AI|Combat")
	FName CurrentAttackersBlackboard = "CurrentAttackers";

	// Set from the Settings DT
	UPROPERTY() bool bOnlyDamagePlayers = false;
#pragma endregion Combat
};
