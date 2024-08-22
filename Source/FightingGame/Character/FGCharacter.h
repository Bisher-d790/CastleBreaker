#pragma once
#include "GameFramework/Character.h"

// Engine
#include "CoreMinimal.h"

#include "FGCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AEquippableItem;

UCLASS(config = Game)
class AFGCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Overrides
public:
	AFGCharacter();
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

#pragma region Item
public:
	template<class T>
	inline T* GetEquippedItem() const { return Cast<T>(EquippedItem); }

	UFUNCTION(BlueprintPure, Category = "EquippedItem")
	FORCEINLINE AEquippableItem* GetEquippedItem() const { return EquippedItem; }

	void EquipItem(AEquippableItem* Item);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquippedItemChanged, const AEquippableItem*, OldItem, const AEquippableItem*, EquippedItem);
	UPROPERTY(BlueprintAssignable, Category = "EquippedItem")
	FOnEquippedItemChanged OnEquippedItemChanged;

protected:
	UPROPERTY() AEquippableItem* EquippedItem = nullptr;

	// The socket name to connect the item to the Skeleton
	UPROPERTY(EditDefaultsOnly, Category = "EquippedItem|Socket")
	FName EquippedItemSocket = "WeaponSocket";
#pragma endregion Item
};

