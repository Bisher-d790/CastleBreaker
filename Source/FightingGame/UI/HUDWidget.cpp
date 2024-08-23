#include "HUDWidget.h"

// Project
#include "FightingGame/Interfaces/DamageableInterface.h"
#include "FightingGame/Components/HealthComponent.h"


void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (const auto Pawn = GetOwningPlayerPawn())
	{
		if (const auto DamageablePawn = Cast<IDamageableInterface>(Pawn))
		{
			if (const auto HealthComp = DamageablePawn->GetHealthComponent())
			{
				if (!HealthComp->OnHealthChanged.IsAlreadyBound(this, &ThisClass::UpdateHealth))
					HealthComp->OnHealthChanged.AddDynamic(this, &ThisClass::UpdateHealth);

				MaxHealth = HealthComp->GetMaxHealth();
				UpdateHealth(HealthComp->GetHealth(), 0);
			}
		}
	}
}

void UHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (const auto Pawn = GetOwningPlayerPawn())
	{
		if (const auto DamageablePawn = Cast<IDamageableInterface>(Pawn))
		{
			if (const auto HealthComp = DamageablePawn->GetHealthComponent())
			{
				if (HealthComp->OnHealthChanged.IsAlreadyBound(this, &ThisClass::UpdateHealth))
					HealthComp->OnHealthChanged.RemoveDynamic(this, &ThisClass::UpdateHealth);
			}
		}
	}
}