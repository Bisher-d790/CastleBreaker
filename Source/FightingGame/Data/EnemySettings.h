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
	float PatrolRadius = 1500.f;
};
