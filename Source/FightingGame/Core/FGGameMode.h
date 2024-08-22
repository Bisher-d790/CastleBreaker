#pragma once
#include "GameFramework/GameModeBase.h"

// Engine
#include "CoreMinimal.h"

#include "FGGameMode.generated.h"

UCLASS(minimalapi)
class AFGGameMode : public AGameModeBase
{
	GENERATED_BODY()

#pragma region Overrides
public:
	AFGGameMode() = default;
#pragma endregion Overrides
};



