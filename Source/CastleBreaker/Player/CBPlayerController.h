#pragma once
#include "GameFramework/PlayerController.h"

// Engine
#include "CoreMinimal.h"

#include "CBPlayerController.generated.h"

class AEquippableItem;
class UUserWidget;
class UWaveStartedWidget;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/// <summary>
/// The main Player Contorller, it handles UI, Controls
/// </summary>
UCLASS()
class CASTLEBREAKER_API ACBPlayerController : public APlayerController
{
	GENERATED_BODY()

#pragma region Overrides
public:
	ACBPlayerController() = default;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	virtual void SetupInputComponent() override;
#pragma endregion Overrides

#pragma region Input
protected:
	/** Called for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Input MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> PrimaryActionInputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> SecondaryActionInputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> JumpInputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> CrouchInputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction = nullptr;
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
#pragma endregion Movement

#pragma region GamePhases
protected:
	UFUNCTION() virtual void OnWaveStarted(const int32 WaveNumber);

	UFUNCTION() virtual void OnGameEnded();
#pragma endregion GamePhases

#pragma region UI
protected:
	void SetupHUDWidget();

	UPROPERTY(EditDefaultsOnly, Category = "UI|HUD")
	bool bAddHUDOnBeginPlay = true;

	UPROPERTY(EditDefaultsOnly, Category = "UI|HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass = nullptr;

	UPROPERTY() TObjectPtr<UUserWidget> HUDWidgetInstance = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Wave")
	TSubclassOf<UWaveStartedWidget> WaveStartWidgetClass = nullptr;

	UPROPERTY() TObjectPtr<UWaveStartedWidget> WaveStartWidgetInstance = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI|GameOver")
	TSubclassOf<UUserWidget> GameOverWidgetClass = nullptr;

	UPROPERTY() TObjectPtr<UUserWidget> GameOverWidgetInstance = nullptr;
#pragma endregion UI

#pragma region Death
protected:
	UFUNCTION() void HandlePlayerDeath();

	UPROPERTY(EditDefaultsOnly, Category = "UI|Death")
	TSubclassOf<UUserWidget> DeathWidgetClass = nullptr;

	UPROPERTY() TObjectPtr<UUserWidget> DeathWidgetInstance = nullptr;

	UPROPERTY() bool bIsDead = false;
#pragma endregion Death

#pragma region Kill
public:
	UFUNCTION(BlueprintCallable, Category = "Kill")
	void EnemyKilled(const AActor* KilledEnemy);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyKilled, const AActor*, KilledEnemy);
	FOnEnemyKilled OnEnemyKilled;
#pragma endregion Kill
};
