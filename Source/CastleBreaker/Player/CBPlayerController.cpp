#include "CBPlayerController.h"

// Engine
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

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

	if (!IsValid(GetLocalPlayer()) || !InputMappingContext) return;

	// Add Input Mapping Context
	if (auto* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputSubsystem->AddMappingContext(InputMappingContext, 0);
	}

	// Set up action bindings
	if (auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Set up gameplay key bindings
		// Equipped Item
		EnhancedInputComponent->BindAction(PrimaryActionInputAction, ETriggerEvent::Started, this, &ACBPlayerController::OnPrimaryActionStart);
		EnhancedInputComponent->BindAction(PrimaryActionInputAction, ETriggerEvent::Completed, this, &ACBPlayerController::OnPrimaryActionEnd);
		EnhancedInputComponent->BindAction(SecondaryActionInputAction, ETriggerEvent::Started, this, &ACBPlayerController::OnSecondaryActionStart);
		EnhancedInputComponent->BindAction(SecondaryActionInputAction, ETriggerEvent::Completed, this, &ACBPlayerController::OnSecondaryActionEnd);

		// Jump
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ACBPlayerController::CharacterJump);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchInputAction, ETriggerEvent::Started, this, &ACBPlayerController::CharacterCrouchToggle);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACBPlayerController::MoveInput);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACBPlayerController::LookInput);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACBPlayerController::MoveInput(const FInputActionValue& Value)
{
	if (const auto OwnedCharacter = GetPawn<ACBCharacter>())
	{
		// input is a Vector2D
		const FVector2D MovementVector = Value.Get<FVector2D>();

		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		OwnedCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
		OwnedCharacter->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACBPlayerController::LookInput(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// add yaw and pitch input to controller
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
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
