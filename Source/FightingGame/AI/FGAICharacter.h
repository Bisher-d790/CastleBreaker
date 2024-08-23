#pragma once
#include "FightingGame/Character/FGCharacter.h"

// Engine
#include "CoreMinimal.h"

#include "FGAICharacter.generated.h"

UCLASS()
class FIGHTINGGAME_API AFGAICharacter : public AFGCharacter
{
	GENERATED_BODY()

#pragma region Overrides
public:
	AFGAICharacter();

protected:
	virtual void BeginPlay() override;
#pragma endregion Overrides

#pragma region Health
public:
	virtual void TakeDamage(const float Damage) override;
	virtual void Heal(const float HealAmount) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Health|HealthBar")
	void UpdateHealthBar();
#pragma endregion Health
};
