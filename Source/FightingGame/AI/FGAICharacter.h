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
#pragma endregion Overrides
};
