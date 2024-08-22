#include "FGPlayerController.h"

// Project
#include "FightingGame/Character/FGCharacter.h"


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
	if (const auto FGCharacter = GetPawn<AFGCharacter>())
		FGCharacter->StartPrimaryAction();
}

void AFGPlayerController::OnPrimaryActionEnd()
{
	if (const auto FGCharacter = GetPawn<AFGCharacter>())
		FGCharacter->StopPrimaryAction();
}

void AFGPlayerController::OnSecondaryActionStart()
{
	if (const auto FGCharacter = GetPawn<AFGCharacter>())
		FGCharacter->StartSecondaryAction();
}

void AFGPlayerController::OnSecondaryActionEnd()
{
	if (const auto FGCharacter = GetPawn<AFGCharacter>())
		FGCharacter->StopSecondaryAction();
}

void AFGPlayerController::CharacterJump()
{
	if (!IsValid(GetCharacter())) return;

	GetCharacter()->Jump();
}

void AFGPlayerController::CharacterCrouchToggle()
{
	if (!IsValid(GetCharacter())) return;

	if (GetCharacter()->bIsCrouched)
		GetCharacter()->UnCrouch();
	else
		GetCharacter()->Crouch();
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
