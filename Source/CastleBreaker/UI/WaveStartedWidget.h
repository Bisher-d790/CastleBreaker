#pragma once
#include "Blueprint/UserWidget.h"

// Engine
#include "CoreMinimal.h"

#include "WaveStartedWidget.generated.h"

UCLASS()
class CASTLEBREAKER_API UWaveStartedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Waves")
	void SetWaveNumber(const int32 WaveNumber);

	UFUNCTION(BlueprintImplementableEvent, Category = "Waves")
	void ShowWidget();
};
