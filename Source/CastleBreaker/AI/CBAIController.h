#pragma once
#include "AIController.h"

// Engine
#include "CoreMinimal.h"

#include "CBAIController.generated.h"

class UDataTable;
class UBehaviorTree;

/// <summary>
/// Parent AI Contorller class to any AI Agent type, handles settings the settings, running the BT, and basic functionalities
/// </summary>
UCLASS()
class CASTLEBREAKER_API ACBAIController : public AAIController
{
	GENERATED_BODY()

#pragma region Overrides
public:
	ACBAIController() = default;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
#pragma endregion Overrides

#pragma region AI
protected:
	// The main behavior to run
	UPROPERTY(EditAnywhere, Category = "AI|BehaviorTree")
	UBehaviorTree* BehaviorTree = nullptr;
#pragma endregion AI

#pragma region Settings
protected:
	virtual void SetupSettingsFromDT() {};

	UPROPERTY(EditDefaultsOnly, Category = "AI|Settings")
	UDataTable* SettingsTable = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Settings")
	FString SettingsRow = "Default";
#pragma endregion Settings

#pragma region Death
protected:
	UFUNCTION() void HandleDeath();

	// Time to destroy actor after death, could be used for pooling later.
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float DestroyTimeAfterDeath = 2.f;
#pragma region Death
};
