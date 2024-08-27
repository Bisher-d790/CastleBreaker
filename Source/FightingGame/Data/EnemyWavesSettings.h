#pragma once
#include "Engine/DataTable.h"

// Engine
#include "CoreMinimal.h"

#include "EnemyWavesSettings.generated.h"

class AFGAICharacter;

USTRUCT(BlueprintType, Blueprintable)
struct FEnemies : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Enemy")
	TSubclassOf<AFGAICharacter> EnemyClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	int32 EnemyCount = 10;
};

USTRUCT(BlueprintType, Blueprintable)
struct FEnemyWave : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Wave")
	TArray<FEnemies> Enemies = {};

	UPROPERTY(EditAnywhere, Category = "Wave")
	float WaveStartDelay = 2.f;

	UPROPERTY(EditAnywhere, Category = "Wave")
	float TimeBetweenSpawns = 0.5f;
};
