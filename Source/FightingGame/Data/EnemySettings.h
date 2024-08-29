#pragma once
#include "Engine/DataTable.h"

// Engine
#include "CoreMinimal.h"

#include "EnemySettings.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct FEnemySettings : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float MinPatrolRadius = 300.f;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float MaxPatrolRadius = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bOnlyCanDamagePlayers = true;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MinCombatRange = 100.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxCombatRange = 500.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackDistance = 100.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int MaxConcurrentAttackers = 1;
};
