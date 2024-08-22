#pragma once
#include "GameFramework/Character.h"

// Engine
#include "CoreMinimal.h"

#include "FGCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

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
};

