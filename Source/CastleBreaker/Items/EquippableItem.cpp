#include "EquippableItem.h"

// Engine
#include "Components/BoxComponent.h"

// Project


AEquippableItem::AEquippableItem()
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Collision->SetSimulatePhysics(true);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionResponseToAllChannels(ECR_Block);
	SetRootComponent(Collision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetupAttachment(RootComponent);
}

void AEquippableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPrimaryActionActive)
	{
		PrimaryActionTimeActive += DeltaTime;
	}

	if (bIsSecondaryActionActive)
	{
		SecondaryActionTimeActive += DeltaTime;
	}
}

void AEquippableItem::SetItemSimulatePhysics(const bool bIsEnabled)
{
	if (!IsValid(Collision)) return;

	if (bIsEnabled)
	{
		Collision->SetSimulatePhysics(true);
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		Collision->SetSimulatePhysics(false);
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEquippableItem::OnItemEquipped(AActor* OwnerActor)
{
	if (!IsValid(OwnerActor))
	{
		OnItemUnequipped();
		return;
	}

	SetOwner(OwnerActor);

	SetItemSimulatePhysics(false);
}

void AEquippableItem::OnItemUnequipped()
{
	SetOwner(nullptr);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	SetItemSimulatePhysics(true);
}

void AEquippableItem::StartPrimaryAction()
{
	PrimaryActionTimeActive = 0.f;
	bIsPrimaryActionActive = true;

	OnPrimaryAction.Broadcast(true);
}

void AEquippableItem::StopPrimaryAction()
{
	bIsPrimaryActionActive = false;
	PrimaryActionTimeActive = 0.f;

	OnPrimaryAction.Broadcast(false);
}

void AEquippableItem::StartSecondaryAction()
{
	SecondaryActionTimeActive = 0.f;
	bIsSecondaryActionActive = true;

	OnSecondaryAction.Broadcast(true);
}

void AEquippableItem::StopSecondaryAction()
{
	bIsSecondaryActionActive = false;
	SecondaryActionTimeActive = 0.f;

	OnSecondaryAction.Broadcast(false);
}
