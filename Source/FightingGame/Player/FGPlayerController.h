#pragma once
#include "GameFramework/PlayerController.h"

// Engine
#include "CoreMinimal.h"

#include "FGPlayerController.generated.h"

class AEquippableItem;
class UUserWidget;

UCLASS()
class FIGHTINGGAME_API AFGPlayerController : public APlayerController
{
	GENERATED_BODY()

#pragma region Overrides
public:
	AFGPlayerController() = default;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	virtual void SetupInputComponent() override;
#pragma endregion Overrides

#pragma region Camera
protected:
	void TurnAtRate(const float Rate);

	void LookUpAtRate(const float Rate);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate = 45.f;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate = 45.f;
#pragma endregion Camera

#pragma region Input
protected:
	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName PrimaryActionInputName = "PrimaryAction";

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName SecondaryActionInputName = "SecondaryAction";

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName JumpInputName = "Jump";

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName CrouchInputName = "Crouch";

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName MoveForwardInputName = "MoveForward";

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName MoveRightInputName = "MoveRight";

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName TurnInputName = "Turn";

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName TurnRateInputName = "TurnRate";

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName LookUpInputName = "LookUp";

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName LookUpRateInputName = "LookUpRate";
#pragma endregion Input

#pragma region Actions
protected:
	void OnPrimaryActionStart();
	void OnPrimaryActionEnd();
	void OnSecondaryActionStart();
	void OnSecondaryActionEnd();
#pragma endregion Actions

#pragma region Movement
protected:
	/** Called for jump input */
	void CharacterJump();

	void CharacterCrouchToggle();

	/** Called for forwards/backward input */
	void CharacterMoveForward(const float Value);

	/** Called for side to side input */
	void CharacterMoveRight(const float Value);
#pragma endregion Movement

#pragma region EquippableItem
public:
	AEquippableItem* SpawnAndEquipNewItem(TSubclassOf<AEquippableItem> Item);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "EquippableItem")
	TSubclassOf<AEquippableItem> ItemToEquipOnBeginPlay = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "EquippableItem")
	bool bEquipItemOnBeginPlay = true;
#pragma endregion EquippableItem

#pragma region UI
protected:
	void SetupHUDWidget();

	UPROPERTY(EditDefaultsOnly, Category = "UI|HUD")
	bool bAddHUDOnBeginPlay = true;

	UPROPERTY(EditDefaultsOnly, Category = "UI|HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass = nullptr;

	UPROPERTY() UUserWidget* HUDWidgetInstance = nullptr;
#pragma endregion UI

#pragma region Death
protected:
	UFUNCTION() void HandlePlayerDeath();

	UPROPERTY(EditDefaultsOnly, Category = "UI|Death")
	TSubclassOf<UUserWidget> DeathWidgetClass = nullptr;

	UPROPERTY() UUserWidget* DeathWidgetInstance = nullptr;

	UPROPERTY() bool bIsDead = false;
#pragma endregion Death
};
