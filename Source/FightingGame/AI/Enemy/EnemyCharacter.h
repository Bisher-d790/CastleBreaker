#pragma once
#include "FightingGame/AI/FGAICharacter.h"

// Engine
#include "CoreMinimal.h"

#include "EnemyCharacter.generated.h"

UCLASS()
class FIGHTINGGAME_API AEnemyCharacter : public AFGAICharacter
{
	GENERATED_BODY()

#pragma region Sensing
protected:
	virtual void HandlePawnSeen(APawn* Pawn) override;
#pragma endregion Sensing
};
