#include "CBPlayerController.h"

// Engine
#include "Blueprint/UserWidget.h"

// Project
#include "CastleBreaker/Character/CBCharacter.h"
#include "CastleBreaker/Items/EquippableItem.h"
#include "CastleBreaker/Components/HealthComponent.h"
#include "CastleBreaker/Player/CBPlayerState.h"
#include "CastleBreaker/Core/CBGameMode.h"
#include "CastleBreaker/UI/WaveStartedWidget.h"


void ACBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (const auto GM = GetWorld()->GetAuthGameMode<ACBGameMode>())
	{
		GM->OnWaveStarted.AddDynamic(this, &ThisClass::OnWaveStarted);
		GM->OnGameOver.AddDynamic(this, &ThisClass::OnGameEnded);
	}
}

void ACBPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (const auto OwnedCharacter = Cast<ACBCharacter>(InPawn))
	{
		if (const auto HealthComponent = OwnedCharacter->GetHealthComponent())
			if (!HealthComponent->OnDeath.IsAlreadyBound(this, &ThisClass::HandlePlayerDeath))
				HealthComponent->OnDeath.AddDynamic(this, &ThisClass::HandlePlayerDeath);
	}

	if (bAddHUDOnBeginPlay)
		SetupHUDWidget();
}

void ACBPlayerController::OnUnPossess()
{
	if (const auto OwnedCharacter = GetPawn<ACBCharacter>())
	{
		if (const auto HealthComponent = OwnedCharacter->GetHealthComponent())
			if (HealthComponent->OnDeath.IsAlreadyBound(this, &ThisClass::HandlePlayerDeath))
				HealthComponent->OnDeath.RemoveDynamic(this, &ThisClass::HandlePlayerDeath);
	}

	Super::OnUnPossess();
}

void ACBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!IsValid(InputComponent)) return;

	// Set up gameplay key bindings
	InputComponent->BindAction(PrimaryActionInputName, IE_Pressed, this, &ACBPlayerController::OnPrimaryActionStart);
	InputComponent->BindAction(PrimaryActionInputName, IE_Released, this, &ACBPlayerController::OnPrimaryActionEnd);
	InputComponent->BindAction(SecondaryActionInputName, IE_Pressed, this, &ACBPlayerController::OnSecondaryActionStart);
	InputComponent->BindAction(SecondaryActionInputName, IE_Released, this, &ACBPlayerController::OnSecondaryActionEnd);

	InputComponent->BindAction(JumpInputName, IE_Pressed, this, &ACBPlayerController::CharacterJump);

	InputComponent->BindAction(CrouchInputName, IE_Pressed, this, &ACBPlayerController::CharacterCrouchToggle);

	InputComponent->BindAxis(MoveForwardInputName, this, &ACBPlayerController::CharacterMoveForward);
	InputComponent->BindAxis(MoveRightInputName, this, &ACBPlayerController::CharacterMoveRight);

	InputComponent->BindAxis(TurnInputName, this, &ACBPlayerController::AddYawInput);
	InputComponent->BindAxis(TurnRateInputName, this, &ACBPlayerController::TurnAtRate);
	InputComponent->BindAxis(LookUpInputName, this, &ACBPlayerController::AddPitchInput);
	InputComponent->BindAxis(LookUpRateInputName, this, &ACBPlayerController::LookUpAtRate);
}

void ACBPlayerController::TurnAtRate(const float Rate)
{
	// calculate delta for this frame from the rate information
	AddYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACBPlayerController::LookUpAtRate(const float Rate)
{
	// calculate delta for this frame from the rate information
	AddPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACBPlayerController::OnPrimaryActionStart()
{
	if (const auto OwnedCharacter = GetPawn<ACBCharacter>())
		OwnedCharacter->StartPrimaryAction();
}

void ACBPlayerController::OnPrimaryActionEnd()
{
	if (const auto OwnedCharacter = GetPawn<ACBCharacter>())
		OwnedCharacter->StopPrimaryAction();
}

void ACBPlayerController::OnSecondaryActionStart()
{
	if (const auto OwnedCharacter = GetPawn<ACBCharacter>())
		OwnedCharacter->StartSecondaryAction();
}

void ACBPlayerController::OnSecondaryActionEnd()
{
	if (const auto OwnedCharacter = GetPawn<ACBCharacter>())
		OwnedCharacter->StopSecondaryAction();
}

void ACBPlayerController::CharacterJump()
{
	if (const auto OwnedCharacter = GetPawn<ACBCharacter>())
		OwnedCharacter->Jump();
}

void ACBPlayerController::CharacterCrouchToggle()
{
	if (const auto OwnedCharacter = GetPawn<ACBCharacter>())
	{
		if (OwnedCharacter->bIsCrouched)
			OwnedCharacter->UnCrouch();
		else
			OwnedCharacter->Crouch();
	}
}

void ACBPlayerController::CharacterMoveForward(const float Value)
{
	if (!IsValid(GetCharacter()) || Value == 0.0f) return;

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// add movement in that direction
	GetCharacter()->AddMovementInput(Direction, Value);
}

void ACBPlayerController::CharacterMoveRight(const float Value)
{
	if (!IsValid(GetCharacter()) || Value == 0.0f) return;

	// find out which way is right
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// add movement in that direction
	GetCharacter()->AddMovementInput(Direction, Value);
}

void ACBPlayerController::OnWaveStarted(const int32 WaveNumber)
{
	if (!IsValid(WaveStartWidgetClass)) return;

	if (!IsValid(WaveStartWidgetInstance))
	{
		WaveStartWidgetInstance = CreateWidget<UWaveStartedWidget>(this, WaveStartWidgetClass);
	}

	if (IsValid(WaveStartWidgetInstance))
	{
		if (!WaveStartWidgetInstance->IsInViewport())
			WaveStartWidgetInstance->AddToViewport();

		WaveStartWidgetInstance->SetWaveNumber(WaveNumber);
		WaveStartWidgetInstance->ShowWidget();
	}
}

void ACBPlayerController::OnGameEnded()
{
	if (!IsValid(GameOverWidgetClass)) return;

	if (!IsValid(GameOverWidgetInstance))
	{
		GameOverWidgetInstance = CreateWidget(this, GameOverWidgetClass);
	}

	if (IsValid(GameOverWidgetInstance))
	{
		if (!GameOverWidgetInstance->IsInViewport())
			GameOverWidgetInstance->AddToViewport();

		bShowMouseCursor = true;
	}
}

void ACBPlayerController::SetupHUDWidget()
{
	if (!IsValid(HUDWidgetClass)) return;

	if (!IsValid(HUDWidgetInstance))
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
	}

	if (IsValid(HUDWidgetInstance))
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance->AddToViewport();
	}
}

void ACBPlayerController::HandlePlayerDeath()
{
	if (bIsDead) return;
	bIsDead = true;

	if (IsValid(HUDWidgetInstance))
	{
		HUDWidgetInstance->RemoveFromParent();
	}

	if (!IsValid(DeathWidgetInstance))
	{
		DeathWidgetInstance = CreateWidget<UUserWidget>(this, DeathWidgetClass);
	}

	if (IsValid(DeathWidgetInstance))
	{
		DeathWidgetInstance->AddToViewport();

		bShowMouseCursor = true;
	}

	if (const auto OwnedCharacter = GetPawn<ACBCharacter>())
	{
		OwnedCharacter->UnEquipItem();

		if (const auto CharacterMesh = OwnedCharacter->GetMesh())
			CharacterMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}

	DisableInput(this);
}

void ACBPlayerController::EnemyKilled(const AActor* KilledEnemy)
{
	if (const auto PS = GetPlayerState<ACBPlayerState>())
	{
		PS->AddKills(1);

		OnEnemyKilled.Broadcast(KilledEnemy);
	}
}
