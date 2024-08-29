#pragma once
#include "UObject/Interface.h"

// Engine
#include "CoreMinimal.h"

#include "DamageableInterface.generated.h"

class UHealthComponent;

/// <summary>
/// The interface by which we interact with the Health Component and damage actors
/// Implementors must have a Health Component
/// </summary>
UINTERFACE(MinimalAPI, Blueprintable)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

class IDamageableInterface
{
	GENERATED_BODY()

public:
	virtual void TakeDamage(const float Damage) = 0;

	virtual void Heal(const float HealAmount) = 0;

	virtual float GetHealth() const { return 0.f; };

	virtual FORCEINLINE UHealthComponent* GetHealthComponent() const { return nullptr; };
};