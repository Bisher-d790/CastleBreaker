#pragma once
#include "GameFramework/Actor.h"

// Engine
#include "CoreMinimal.h"

#include "AIAgentFactory.generated.h"

class ACBAICharacter;
class UArrowComponent;

/// <summary>
/// Factory class to spawn AI Agents in the Map
/// </summary>
UCLASS()
class CASTLEBREAKER_API AAIAgentFactory : public AActor
{
	GENERATED_BODY()

#pragma region Overrides
public:
	AAIAgentFactory();
#pragma endregion Overrides

#pragma region Component
protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UArrowComponent> ArrowComponent = nullptr;
#pragma endregion Component

#pragma region Spawning
public:
	ACBAICharacter* SpawnAgent(TSubclassOf<ACBAICharacter> AgentClass);
#pragma endregion Spawning
};
