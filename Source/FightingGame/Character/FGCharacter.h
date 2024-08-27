#pragma once
#include "GameFramework/Character.h"

// Engine
#include "CoreMinimal.h"

// Project
#include "FightingGame/Interfaces/DamageableInterface.h"

#include "FGCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UHealthComponent;
class AEquippableItem;

UCLASS(config = Game)
class AFGCharacter : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()

#pragma region Overrides
public:
	AFGCharacter();

protected:
	virtual void BeginPlay() override;

	virtual float InternalTakeRadialDamage(float Damage, struct FRadialDamageEvent const& RadialDamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	virtual float InternalTakePointDamage(float Damage, struct FPointDamageEvent const& PointDamageEvent, class AController* EventInstigator, AActor* DamageCauser);
#pragma endregion Overrides

#pragma region Camera
protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom = nullptr;

	/** Follow camera */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera = nullptr;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
#pragma region Camera

#pragma region Actions
public:
	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual void StartPrimaryAction();

	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual void StopPrimaryAction();

	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual void StartSecondaryAction();

	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual void StopSecondaryAction();
#pragma endregion Actions

#pragma region EquippedItem
public:
	template<class T>
	inline T* GetEquippedItem() const { return Cast<T>(EquippedItem); }

	UFUNCTION(BlueprintPure, Category = "EquippedItem")
	FORCEINLINE AEquippableItem* GetEquippedItem() const { return EquippedItem; }

	AEquippableItem* SpawnAndEquipNewItem(TSubclassOf<AEquippableItem> Item);

	void EquipItem(AEquippableItem* Item);
	void UnEquipItem();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquippedItemChanged, const AEquippableItem*, OldItem, const AEquippableItem*, EquippedItem);
	UPROPERTY(BlueprintAssignable, Category = "EquippedItem")
	FOnEquippedItemChanged OnEquippedItemChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "EquippedItem")
	TSubclassOf<AEquippableItem> ItemToEquipOnBeginPlay = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EquippedItem")
	bool bEquipItemOnBeginPlay = true;

	UPROPERTY() AEquippableItem* EquippedItem = nullptr;

	// The socket name to connect the item to the Skeleton
	UPROPERTY(EditDefaultsOnly, Category = "EquippedItem|Socket")
	FName EquippedItemSocket = "WeaponSocket";
#pragma endregion EquippedItem

#pragma region Health
public:
	UFUNCTION(BlueprintPure, Category = "Health")
	virtual FORCEINLINE UHealthComponent* GetHealthComponent() const override { return HealthComponent; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void TakeDamage(const float Damage) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void Heal(const float HealAmount) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual float GetHealth() const override;

	// Update the health bar that is visible only for other players
	UFUNCTION(BlueprintImplementableEvent, Category = "Health|HealthBar")
	void UpdateHealthBar();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	UHealthComponent* HealthComponent = nullptr;
#pragma endregion Health
};

