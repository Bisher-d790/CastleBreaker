#pragma once
#include "Engine/DataTable.h"

// Engine
#include "CoreMinimal.h"

#include "EnemySettings.generated.h"

/// <summary>
/// Main Enemy Settings, to be used in a Data Table
/// </summary>
USTRUCT(BlueprintType, Blueprintable)
struct FEnemySettings : public FTableRowBase
{
	GENERATED_BODY()

public:
	// Non-Combat, Min radius to patrol around current location
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float MinPatrolRadius = 300.f;

	// Non-Combat, Max radius to patrol around current location
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float MaxPatrolRadius = 1500.f;

	// Health to be considered Low
	UPROPERTY(EditAnywhere, Category = "Health")
	float LowHealthThreshold = 30.f;

	// Whether the agent can only damage players and not other AI
	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bOnlyCanDamagePlayers = true;

	// Combat, the min distance to be around the enemy when not attacking
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MinCombatRange = 100.f;

	// Combat, the max distance to be around the enemy when not attacking
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxCombatRange = 500.f;

	// During attack, the distance to the player that Enemy will get close to before performing the attack
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackDistance = 100.f;

	// Max number of Enemies attacking the player simultanuously
	UPROPERTY(EditAnywhere, Category = "Combat")
	int MaxConcurrentAttackers = 1;
};
