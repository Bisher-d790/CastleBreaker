#pragma once
#include "FightingGame/Character/FGCharacter.h"

// Engine
#include "CoreMinimal.h"

#include "FGAICharacter.generated.h"

class UPawnSensingComponent;
class UNavigationInvokerComponent;

/// <summary>
/// Parent Character class to any AI type, have basic functionality that any AI Agent Character needs
/// </summary>
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

#pragma region Components
public:
	UFUNCTION(BlueprintPure, Category = "AI")
	FORCEINLINE UPawnSensingComponent* GetPawnSensingComponent() const { return PawnSensingComponent; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UPawnSensingComponent* PawnSensingComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UNavigationInvokerComponent* NavigationInvokerComponent = nullptr;
#pragma endregion Components

#pragma region Sensing
protected:
	UFUNCTION() virtual void HandlePawnSeen(APawn* Pawn) {};
#pragma endregion Sensing
};
