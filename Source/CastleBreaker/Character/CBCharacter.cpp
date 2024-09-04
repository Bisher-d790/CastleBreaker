#include "CBCharacter.h"

// Engine
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Project
#include "CastleBreaker/Items/EquippableItem.h"
#include "CastleBreaker/Components/HealthComponent.h"


ACBCharacter::ACBCharacter()
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

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ACBCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HealthComponent))
	{
		UpdateHealthBar(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
	}

	if (bEquipItemOnBeginPlay)
	{
		SpawnAndEquipNewItem(ItemToEquipOnBeginPlay);
	}
}

float ACBCharacter::InternalTakeRadialDamage(float Damage, FRadialDamageEvent const& RadialDamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::InternalTakeRadialDamage(Damage, RadialDamageEvent, EventInstigator, DamageCauser);
	TakeDamage(ActualDamage);
	return ActualDamage;
}

float ACBCharacter::InternalTakePointDamage(float Damage, FPointDamageEvent const& PointDamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::InternalTakePointDamage(Damage, PointDamageEvent, EventInstigator, DamageCauser);
	TakeDamage(ActualDamage);
	return ActualDamage;
}

void ACBCharacter::StartPrimaryAction()
{
	if (IsValid(EquippedItem))
		EquippedItem->StartPrimaryAction();
}

void ACBCharacter::StopPrimaryAction()
{
	if (IsValid(EquippedItem))
		EquippedItem->StopPrimaryAction();
}

void ACBCharacter::StartSecondaryAction()
{
	if (IsValid(EquippedItem))
		EquippedItem->StartSecondaryAction();
}

void ACBCharacter::StopSecondaryAction()
{
	if (IsValid(EquippedItem))
		EquippedItem->StopSecondaryAction();
}


AEquippableItem* ACBCharacter::SpawnAndEquipNewItem(TSubclassOf<AEquippableItem> Item)
{
	const auto World = GetWorld();
	if (!IsValid(Item) || !IsValid(World)) return nullptr;

	if (const auto SpawnedItem = World->SpawnActor<AEquippableItem>(Item))
	{
		EquipItem(SpawnedItem);

		return SpawnedItem;
	}

	return nullptr;
}

void ACBCharacter::EquipItem(AEquippableItem* Item)
{
	if (!IsValid(Item) || EquippedItem == Item) return;

	const auto OldItem = EquippedItem;
	EquippedItem = Item;

	EquippedItem->OnItemEquipped(this);

	if (IsValid(GetMesh()))
	{
		const FVector& HandleLocation = EquippedItem->GetHandleRelativeLocation();
		EquippedItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, EquippedItemSocket);
		EquippedItem->SetActorRelativeLocation(HandleLocation);
	}

	OnEquippedItemChanged.Broadcast(OldItem, Item);
}

void ACBCharacter::UnEquipItem()
{
	if (!IsValid(EquippedItem)) return;

	EquippedItem->OnItemUnequipped();

	const auto Item = EquippedItem;
	EquippedItem = nullptr;

	OnEquippedItemChanged.Broadcast(Item, EquippedItem);
}

void ACBCharacter::TakeDamage(const float Damage)
{
	if (IsValid(HealthComponent))
	{
		HealthComponent->Damage(Damage);

		UpdateHealthBar(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
	}
}

void ACBCharacter::Heal(const float HealAmount)
{
	if (IsValid(HealthComponent))
	{
		HealthComponent->Heal(HealAmount);

		UpdateHealthBar(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
	}
}

float ACBCharacter::GetHealth() const
{
	if (IsValid(HealthComponent))
	{
		return HealthComponent->GetHealth();
	}

	return 0.0f;
}
