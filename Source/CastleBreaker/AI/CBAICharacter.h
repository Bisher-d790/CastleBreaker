#pragma once
#include "CastleBreaker/Character/CBCharacter.h"

// Engine
#include "CoreMinimal.h"

#include "CBAICharacter.generated.h"

class UPawnSensingComponent;
class UNavigationInvokerComponent;

/// <summary>
/// Parent Character class to any AI type, have basic functionality that any AI Agent Character needs
/// </summary>
UCLASS()
class CASTLEBREAKER_API ACBAICharacter : public ACBCharacter
{
	GENERATED_BODY()

#pragma region Overrides
public:
	ACBAICharacter();

protected:
	virtual void BeginPlay() override;
#pragma endregion Overrides

#pragma region Components
public:
	UFUNCTION(BlueprintPure, Category = "AI")
	FORCEINLINE UPawnSensingComponent* GetPawnSensingComponent() const { return PawnSensingComponent; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UNavigationInvokerComponent> NavigationInvokerComponent = nullptr;
#pragma endregion Components

#pragma region Sensing
protected:
	UFUNCTION() virtual void HandlePawnSeen(APawn* Pawn) {};
#pragma endregion Sensing
};
