#pragma once
#include "UObject/Interface.h"

// Engine
#include "CoreMinimal.h"

#include "DamageableInterface.generated.h"

class UHealthComponent;

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

	virtual FORCEINLINE UHealthComponent* GetHealthComponent() const { return nullptr; };
};