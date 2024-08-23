#pragma once
#include "FightingGame/Items/EquippableItem.h"

// Engine
#include "CoreMinimal.h"

#include "WeaponItem.generated.h"

UCLASS()
class FIGHTINGGAME_API AWeaponItem : public AEquippableItem
{
	GENERATED_BODY()

#pragma region Attack
public:
	virtual void StartPrimaryAction() override;

	virtual void StartAttack();

	virtual void FinishAttack();

	UFUNCTION(BlueprintPure, Category = "Attack")
	FORCEINLINE bool IsAttacking() const { return bIsAttacking; }

	UFUNCTION(BlueprintPure, Category = "Attack")
	FORCEINLINE float GetAttackDuration() const { return AttackDuration; }

protected:
	virtual bool CanAttack() const;

	UPROPERTY() bool bIsAttacking = false;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackDuration = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	bool bCanAttackInTheAir = false;

	FTimerHandle AttackTimer;
#pragma endregion Attack

	// TODO: Make a comprehensive actions Data Table and set specific animations and properties to each attack
#pragma region Animations
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attack|Animations")
	UAnimationAsset* AttackAnimation = nullptr;
#pragma endregion Animations
};
