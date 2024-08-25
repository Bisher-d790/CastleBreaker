#pragma once
#include "GameFramework/PlayerState.h"

// Engine
#include "CoreMinimal.h"

#include "FGPlayerState.generated.h"

UCLASS()
class FIGHTINGGAME_API AFGPlayerState : public APlayerState
{
	GENERATED_BODY()

#pragma region Kills
public:
	UFUNCTION(BlueprintPure, Category = "Kills")
	FORCEINLINE int32 GetKillCount() const { return Kills; }

	UFUNCTION(BlueprintCallable, Category = "Kills")
	void AddKills(const int32 AddedKills) { Kills += AddedKills; }

	UFUNCTION(BlueprintCallable, Category = "Kills")
	void ResetKills() { Kills = 0; }

protected:
	UPROPERTY() int32 Kills = 0;
#pragma region Kills
};
