#include "FGPlayerController.h"

// Engine
#include "Blueprint/UserWidget.h"

// Project
#include "FightingGame/Character/FGCharacter.h"
#include "FightingGame/Items/EquippableItem.h"
#include "FightingGame/Components/HealthComponent.h"
#include "FightingGame/Player/FGPlayerState.h"


void AFGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (bEquipItemOnBeginPlay)
		SpawnAndEquipNewItem(ItemToEquipOnBeginPlay);

	if (bAddHUDOnBeginPlay)
		SetupHUDWidget();
}

void AFGPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (const auto OwnedCharacter = Cast<AFGCharacter>(InPawn))
	{
		if (const auto HealthComponent = OwnedCharacter->GetHealthComponent())
			if (!HealthComponent->OnDeath.IsAlreadyBound(this, &ThisClass::HandlePlayerDeath))
				HealthComponent->OnDeath.AddDynamic(this, &ThisClass::HandlePlayerDeath);
	}
}

void AFGPlayerController::OnUnPossess()
{
	if (const auto OwnedCharacter = GetPawn<AFGCharacter>())
	{
		if (const auto HealthComponent = OwnedCharacter->GetHealthComponent())
			if (HealthComponent->OnDeath.IsAlreadyBound(this, &ThisClass::HandlePlayerDeath))
				HealthComponent->OnDeath.RemoveDynamic(this, &ThisClass::HandlePlayerDeath);
	}

	Super::OnUnPossess();
}

void AFGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!IsValid(InputComponent)) return;

	// Set up gameplay key bindings
	InputComponent->BindAction(PrimaryActionInputName, IE_Pressed, this, &AFGPlayerController::OnPrimaryActionStart);
	InputComponent->BindAction(PrimaryActionInputName, IE_Released, this, &AFGPlayerController::OnPrimaryActionEnd);
	InputComponent->BindAction(SecondaryActionInputName, IE_Pressed, this, &AFGPlayerController::OnSecondaryActionStart);
	InputComponent->BindAction(SecondaryActionInputName, IE_Released, this, &AFGPlayerController::OnSecondaryActionEnd);

	InputComponent->BindAction(JumpInputName, IE_Pressed, this, &AFGPlayerController::CharacterJump);

	InputComponent->BindAction(CrouchInputName, IE_Pressed, this, &AFGPlayerController::CharacterCrouchToggle);

	InputComponent->BindAxis(MoveForwardInputName, this, &AFGPlayerController::CharacterMoveForward);
	InputComponent->BindAxis(MoveRightInputName, this, &AFGPlayerController::CharacterMoveRight);

	InputComponent->BindAxis(TurnInputName, this, &AFGPlayerController::AddYawInput);
	InputComponent->BindAxis(TurnRateInputName, this, &AFGPlayerController::TurnAtRate);
	InputComponent->BindAxis(LookUpInputName, this, &AFGPlayerController::AddPitchInput);
	InputComponent->BindAxis(LookUpRateInputName, this, &AFGPlayerController::LookUpAtRate);
}

void AFGPlayerController::TurnAtRate(const float Rate)
{
	// calculate delta for this frame from the rate information
	AddYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFGPlayerController::LookUpAtRate(const float Rate)
{
	// calculate delta for this frame from the rate information
	AddPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFGPlayerController::OnPrimaryActionStart()
{
	if (const auto OwnedCharacter = GetPawn<AFGCharacter>())
		OwnedCharacter->StartPrimaryAction();
}

void AFGPlayerController::OnPrimaryActionEnd()
{
	if (const auto OwnedCharacter = GetPawn<AFGCharacter>())
		OwnedCharacter->StopPrimaryAction();
}

void AFGPlayerController::OnSecondaryActionStart()
{
	if (const auto OwnedCharacter = GetPawn<AFGCharacter>())
		OwnedCharacter->StartSecondaryAction();
}

void AFGPlayerController::OnSecondaryActionEnd()
{
	if (const auto OwnedCharacter = GetPawn<AFGCharacter>())
		OwnedCharacter->StopSecondaryAction();
}

void AFGPlayerController::CharacterJump()
{
	if (const auto OwnedCharacter = GetPawn<AFGCharacter>())
		OwnedCharacter->Jump();
}

void AFGPlayerController::CharacterCrouchToggle()
{
	if (const auto OwnedCharacter = GetPawn<AFGCharacter>())
	{
		if (OwnedCharacter->bIsCrouched)
			OwnedCharacter->UnCrouch();
		else
			OwnedCharacter->Crouch();
	}
}

void AFGPlayerController::CharacterMoveForward(const float Value)
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

void AFGPlayerController::CharacterMoveRight(const float Value)
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

AEquippableItem* AFGPlayerController::SpawnAndEquipNewItem(TSubclassOf<AEquippableItem> Item)
{
	const auto World = GetWorld();
	if (!IsValid(Item) || !IsValid(World)) return nullptr;

	if (const auto SpawnedItem = World->SpawnActor<AEquippableItem>(Item))
	{
		if (auto OwnedCharacter = GetPawn<AFGCharacter>())
			OwnedCharacter->EquipItem(SpawnedItem);

		return SpawnedItem;
	}

	return nullptr;
}

void AFGPlayerController::SetupHUDWidget()
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

void AFGPlayerController::HandlePlayerDeath()
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
		DeathWidgetInstance->AddToViewport();

	if (const auto OwnedCharacter = GetPawn<AFGCharacter>())
	{
		OwnedCharacter->UnEquipItem();

		if (const auto CharacterMesh = OwnedCharacter->GetMesh())
			CharacterMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}

	DisableInput(this);
}

void AFGPlayerController::EnemyKilled(const AActor* KilledEnemy)
{
	if (const auto PS = GetPlayerState<AFGPlayerState>())
	{
		PS->AddKills(1);

		OnEnemyKilled.Broadcast(KilledEnemy);
	}
}
