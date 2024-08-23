#include "EquippableItem.h"

// Engine
#include "Components/SphereComponent.h"

// Project


AEquippableItem::AEquippableItem()
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
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
