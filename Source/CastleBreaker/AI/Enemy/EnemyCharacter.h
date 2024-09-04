#pragma once
#include "CastleBreaker/AI/CBAICharacter.h"

// Engine
#include "CoreMinimal.h"

#include "EnemyCharacter.generated.h"

/// <summary>
/// Main class for the Enemy type character, can detect the player
/// </summary>
UCLASS()
class CASTLEBREAKER_API AEnemyCharacter : public ACBAICharacter
{
	GENERATED_BODY()

#pragma region Sensing
protected:
	virtual void HandlePawnSeen(APawn* Pawn) override;
#pragma endregion Sensing
};
