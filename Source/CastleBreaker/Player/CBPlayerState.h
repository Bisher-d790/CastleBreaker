#pragma once
#include "GameFramework/PlayerState.h"

// Engine
#include "CoreMinimal.h"

#include "CBPlayerState.generated.h"

/// <summary>
/// Keeps the Kill Count and other scores
/// </summary>
UCLASS()
class CASTLEBREAKER_API ACBPlayerState : public APlayerState
{
	GENERATED_BODY()

#pragma region KillCount
public:
	UFUNCTION(BlueprintPure, Category = "KillCount")
	FORCEINLINE int32 GetKillCount() const { return KillCount; }

	UFUNCTION(BlueprintCallable, Category = "KillCount")
	void AddKills(const int32 AddedKills) { SetKillCount(KillCount + AddedKills); }

	UFUNCTION(BlueprintCallable, Category = "KillCount")
	void ResetKillCount() { SetKillCount(0); }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKillsChanged, const int32, KillCount);
	FOnKillsChanged OnKillCountChanged;

protected:
	UPROPERTY() int32 KillCount = 0;

private:
	inline void SetKillCount(const int32 Count) {
		KillCount = Count;
		OnKillCountChanged.Broadcast(Count);
	}
#pragma region KillCount
};
