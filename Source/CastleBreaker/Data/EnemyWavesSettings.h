#pragma once
#include "Engine/DataTable.h"

// Engine
#include "CoreMinimal.h"

#include "EnemyWavesSettings.generated.h"

class ACBAICharacter;

// Enemy types in a wave
USTRUCT(BlueprintType, Blueprintable)
struct FEnemies : public FTableRowBase
{
	GENERATED_BODY()

public:
	// The type of enemy to spawn
	UPROPERTY(EditAnywhere, Category = "Enemy")
	TSubclassOf<ACBAICharacter> EnemyClass = nullptr;

	// The number of enemies to spawn of this type
	UPROPERTY(EditAnywhere, Category = "Enemy")
	int32 EnemyCount = 10;
};

/// <summary>
/// Basic Waves settings for the Waves GameMode
/// </summary>
USTRUCT(BlueprintType, Blueprintable)
struct FEnemyWave : public FTableRowBase
{
	GENERATED_BODY()

public:
	// Types of enemies to spawn in this wave
	UPROPERTY(EditAnywhere, Category = "Wave")
	TArray<FEnemies> Enemies = {};

	// Delay before starting the wave
	UPROPERTY(EditAnywhere, Category = "Wave")
	float WaveStartDelay = 2.f;

	// Delay between each Enemy spawn
	UPROPERTY(EditAnywhere, Category = "Wave")
	float TimeBetweenSpawns = 0.5f;
};
