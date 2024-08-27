#include "FGGameMode.h"

// Engine
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"

// Project
#include "FightingGame/AI/AIAgentSpawner.h"
#include "FightingGame/AI/FGAICharacter.h"
#include "FightingGame/Components/HealthComponent.h"


void AFGGameMode::StartPlay()
{
	Super::StartPlay();

	// Start the first wave
	StartWave(StartingWave);
}

void AFGGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Clear timers
	GetWorldTimerManager().ClearAllTimersForObject(this);

	Super::EndPlay(EndPlayReason);
}

void AFGGameMode::GameOver()
{
	OnGameOver.Broadcast();
}

void AFGGameMode::StartWave(const int32 Wave)
{
	CurrentWave = Wave;

	OnWaveStarted.Broadcast(CurrentWave);

	// Initialize all enemy spawner actors in the map
	EnemySpawners.Empty();
	if (IsValid(EnemySpawnerClass))
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemySpawnerClass, EnemySpawners);
	}

	if (EnemySpawners.Num() <= 0) return;

	// Get the wave settings
	const FString& WaveNumber = FString::FromInt(Wave);
	if (const auto Settings = WaveSettings->FindRow<FEnemyWave>(FName(WaveNumber), WaveNumber))
		CurrentWaveSetting = *Settings;

	// Start spawning enemies
	GetWorldTimerManager().SetTimer(SpawnEnemiesTimer, this, &AFGGameMode::SpawnEnemy, CurrentWaveSetting.TimeBetweenSpawns, true, CurrentWaveSetting.WaveStartDelay);
}

void AFGGameMode::EndWave()
{
	OnWaveEnded.Broadcast(CurrentWave);

	if (!IsValid(WaveSettings)) return;

	const int32 NumberOfWaves = WaveSettings->GetRowNames().Num();
	// There's more waves left
	if (CurrentWave < NumberOfWaves)
	{
		StartWave(CurrentWave + 1);
	}
	// No more waves left, GameOver for now
	else
	{
		GameOver();
	}
}

// We will spawn enemies sequentially for now
void AFGGameMode::SpawnEnemy()
{
	if (EnemySpawners.Num() <= 0) return;

	// Find enemy to spawn
	FEnemies* EnemyToSpawn = nullptr;

	int32 EnemyTypeCount = SpawnedEnemies.Num();
	for (auto EnemySettings : CurrentWaveSetting.Enemies)
	{
		// All enemies of this type have been spawned
		if (EnemyTypeCount >= EnemySettings.EnemyCount)
		{
			EnemyTypeCount -= EnemySettings.EnemyCount;
			continue;
		}
		// There's some enemies from this type left to spawn
		else
		{
			EnemyToSpawn = &EnemySettings;
			break;
		}
	}

	// Get a random enemy spawner
	const int32 RandomSpawnerIndex = FMath::RandRange(0, EnemySpawners.Num() - 1);
	if (!EnemyToSpawn
		|| !EnemySpawners.IsValidIndex(RandomSpawnerIndex)
		|| !IsValid(EnemySpawners[RandomSpawnerIndex]))
	{
		GetWorldTimerManager().ClearTimer(SpawnEnemiesTimer);
		return;
	}

	// Spawn the enemy
	if (const auto Spawner = Cast<AAIAgentSpawner>(EnemySpawners[RandomSpawnerIndex]))
	{
		if (const auto SpawnedEnemy = Spawner->SpawnAgent(EnemyToSpawn->EnemyClass))
		{
			SpawnedEnemies.Add(SpawnedEnemy);

			if (const auto EnemyHealthComp = SpawnedEnemy->GetHealthComponent())
			{
				EnemyHealthComp->OnDeath.AddDynamic(this, &AFGGameMode::OnEnemyDeath);
			}
		}
	}
}

void AFGGameMode::OnEnemyDeath()
{
	// Check all enemies health, and check wave end condition
	for (int32 EnemyIndex = 0; EnemyIndex < SpawnedEnemies.Num(); EnemyIndex++)
	{
		if (!IsValid(SpawnedEnemies[EnemyIndex]) || SpawnedEnemies[EnemyIndex]->GetHealth() <= 0)
		{
			if (IsValid(SpawnedEnemies[EnemyIndex]))
			{
				if (const auto EnemyHealthComp = SpawnedEnemies[EnemyIndex]->GetHealthComponent())
				{
					EnemyHealthComp->OnDeath.RemoveDynamic(this, &AFGGameMode::OnEnemyDeath);
				}
			}

			SpawnedEnemies.RemoveAt(EnemyIndex);
		}
	}

	// Check wave end condition
	if (SpawnedEnemies.Num() <= 0)
		EndWave();
}
