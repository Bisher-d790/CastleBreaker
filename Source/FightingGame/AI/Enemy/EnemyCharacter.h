#pragma once
#include "FightingGame/AI/FGAICharacter.h"

// Engine
#include "CoreMinimal.h"

#include "EnemyCharacter.generated.h"

/// <summary>
/// Main class for the Enemy type character, can detect the player
/// </summary>
UCLASS()
class FIGHTINGGAME_API AEnemyCharacter : public AFGAICharacter
{
	GENERATED_BODY()

#pragma region Sensing
protected:
	virtual void HandlePawnSeen(APawn* Pawn) override;
#pragma endregion Sensing
};
