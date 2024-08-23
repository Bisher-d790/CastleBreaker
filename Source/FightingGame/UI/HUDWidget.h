#pragma once
#include "Blueprint/UserWidget.h"

// Engine
#include "CoreMinimal.h"

#include "HUDWidget.generated.h"

UCLASS()
class FIGHTINGGAME_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region Overrides
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
#pragma endregion Overrides

#pragma region HealthBar
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "HealthBar")
	void UpdateHealth(const float Health, const float HealthChange);

	UPROPERTY(BlueprintReadOnly, Category = "HealthBar")
	float MaxHealth = 100.f;
#pragma endregion HealthBar
};
