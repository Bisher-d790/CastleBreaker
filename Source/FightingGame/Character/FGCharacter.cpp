#include "FGCharacter.h"

// Engine
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Project
#include "FightingGame/Items/EquippableItem.h"


AFGCharacter::AFGCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void AFGCharacter::StartPrimaryAction()
{
	if (IsValid(EquippedItem))
		EquippedItem->StartPrimaryAction();
}

void AFGCharacter::StopPrimaryAction()
{
	if (IsValid(EquippedItem))
		EquippedItem->StopPrimaryAction();
}

void AFGCharacter::StartSecondaryAction()
{
	if (IsValid(EquippedItem))
		EquippedItem->StartSecondaryAction();
}

void AFGCharacter::StopSecondaryAction()
{
	if (IsValid(EquippedItem))
		EquippedItem->StopSecondaryAction();
}

void AFGCharacter::EquipItem(AEquippableItem* Item)
{
	if (!IsValid(Item) || EquippedItem == Item) return;

	const auto OldItem = EquippedItem;
	EquippedItem = Item;
	EquippedItem->SetOwner(this);
	if (GetMesh())
		EquippedItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, EquippedItemSocket);

	OnEquippedItemChanged.Broadcast(OldItem, Item);
}