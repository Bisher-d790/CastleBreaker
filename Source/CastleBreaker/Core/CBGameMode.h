#pragma once
#include "GameFramework/GameModeBase.h"

// Engine
#include "CoreMinimal.h"

// Project 
#include "CastleBreaker/Data/EnemyWavesSettings.h"

#include "CBGameMode.generated.h"

class UDataTable;
class AAIAgentFactory;
class ACBAICharacter;

/// <summary>
/// Basic Game Mode class, handles Waves and Spawning enemies
/// </summary>
UCLASS(minimalapi)
class ACBGameMode : public AGameModeBase
{
	GENERATED_BODY()

#pragma region Overrides
public:
	ACBGameMode() = default;

protected:
	virtual void StartPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
#pragma endregion Overrides

#pragma region GamePhases
public:
	virtual void GameOver();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);
	FOnGameOver OnGameOver;
#pragma endregion GamePhases

#pragma region Enemy Waves
public:
	UFUNCTION(BlueprintCallable, Category = "Settings|EnemyWaves")
	void StartWave(const int32 Wave);

	UFUNCTION(BlueprintCallable, Category = "Settings|EnemyWaves")
	void EndWave();

	UFUNCTION(BlueprintPure, Category = "Settings|EnemyWaves")
	FORCEINLINE int32 GetCurrentWave() const { return CurrentWave; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveChanged, const int32, WaveNumber);
	FOnWaveChanged OnWaveStarted;
	FOnWaveChanged OnWaveEnded;

protected:
	UFUNCTION() void SpawnEnemy();

	UFUNCTION() virtual void OnEnemyDeath();

	UPROPERTY(EditDefaultsOnly, Category = "Settings|EnemyWaves")
	int32 StartingWave = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Settings|EnemyWaves")
	TObjectPtr<UDataTable> WaveSettings = nullptr;

	// Type of Enemy Spawner to find in the map
	UPROPERTY(EditDefaultsOnly, Category = "Settings|EnemyWaves")
	TSubclassOf<AAIAgentFactory> EnemySpawnerClass = nullptr;

	// Current references to Spawners
	UPROPERTY() TArray<AActor*> EnemySpawners = {};

	UPROPERTY() TArray<ACBAICharacter*> SpawnedEnemies = {};

	UPROPERTY() int32 CurrentWave = 1;

	UPROPERTY() FEnemyWave CurrentWaveSetting = FEnemyWave();

private:
	FTimerHandle SpawnEnemiesTimer;
#pragma endregion Enemy Waves
};



