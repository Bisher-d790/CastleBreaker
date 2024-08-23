#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(InitialHealth);
}

void UHealthComponent::ChangeHealth(const float ChangeAmount)
{
	SetHealth(Health + ChangeAmount);

	OnHealthChanged.Broadcast(Health, ChangeAmount);

	if (Health <= 0)
	{
		OnHealthReachedZero.Broadcast();
	}
}
