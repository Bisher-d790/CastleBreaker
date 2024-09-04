#pragma once
#include "WeaponItem.h"

// Engine
#include "CoreMinimal.h"

#include "SwordWeapon.generated.h"

class UCapsuleComponent;

// A class of a weapon that can damage actors with a black collision, that is activated and deactivated on attack
UCLASS()
class CASTLEBREAKER_API ASwordWeapon : public AWeaponItem
{
	GENERATED_BODY()

#pragma region Overrides
public:
	ASwordWeapon();

protected:
	virtual void BeginPlay() override;
#pragma endregion Overrides

#pragma region Attack
protected:
	virtual void StartAttack() override;

	virtual void FinishAttack() override;

	UFUNCTION() void HandleBladeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UCapsuleComponent* BladeCollision = nullptr;

	UPROPERTY() TArray<AActor*> AttackedActors = {};
#pragma endregion Attack
};
