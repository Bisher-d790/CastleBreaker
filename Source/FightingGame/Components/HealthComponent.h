#pragma once
#include "Components/ActorComponent.h"

// Engine
#include "CoreMinimal.h"

#include "HealthComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FIGHTINGGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Overrides
public:
	UHealthComponent();

protected:
	virtual void BeginPlay() override;
#pragma endregion Overrides

#pragma region Health
public:
	UFUNCTION(BlueprintPure, Category = "Health|Getters")
	FORCEINLINE float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure, Category = "Health|Getters")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category = "Health|Getters")
	FORCEINLINE bool IsDead() const { return Health <= 0.f; }

	UFUNCTION(BlueprintCallable, Category = "Health|Setters")
	void SetMaxHealth(const float NewMaxHealth) { MaxHealth = NewMaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(const float HealAmount) { ChangeHealth(HealAmount); }

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Damage(const float DamageAmount) { ChangeHealth(-DamageAmount); }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, const float, CurrentHealth, const float, ChangeAmount);
	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnHealthChanged OnHealthChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	UPROPERTY(BlueprintAssignable, Category = "Health|Events")
	FOnDeath OnDeath;

protected:
	void ChangeHealth(const float ChangeAmount);

	inline void SetHealth(const float NewHealth) { Health = FMath::Clamp(NewHealth, 0.f, MaxHealth); }

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float InitialHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float Health = 100.f;
#pragma endregion Health
};
