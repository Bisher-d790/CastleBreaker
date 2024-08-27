#pragma once
#include "GameFramework/GameModeBase.h"

// Engine
#include "CoreMinimal.h"

// Project 
#include "FightingGame/Data/EnemyWavesSettings.h"

#include "FGGameMode.generated.h"

class UDataTable;
class AAIAgentSpawner;
class AFGAICharacter;

UCLASS(minimalapi)
class AFGGameMode : public AGameModeBase
{
	GENERATED_BODY()

#pragma region Overrides
public:
	AFGGameMode() = default;

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
	UDataTable* WaveSettings = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Settings|EnemyWaves")
	TSubclassOf<AAIAgentSpawner> EnemySpawnerClass = nullptr;

	UPROPERTY() TArray<AActor*> EnemySpawners = {};

	UPROPERTY() TArray<AFGAICharacter*> SpawnedEnemies = {};

	UPROPERTY() int32 CurrentWave = 1;

	UPROPERTY() FEnemyWave CurrentWaveSetting = FEnemyWave();

private:
	FTimerHandle SpawnEnemiesTimer;
#pragma endregion Enemy Waves
};



