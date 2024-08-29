#pragma once
#include "GameFramework/Actor.h"

// Engine
#include "CoreMinimal.h"

#include "AIAgentSpawner.generated.h"

class AFGAICharacter;
class UArrowComponent;

/// <summary>
/// Factory class to spawn AI Agents in the Map
/// </summary>
UCLASS()
class FIGHTINGGAME_API AAIAgentSpawner : public AActor
{
	GENERATED_BODY()

#pragma region Overrides
public:
	AAIAgentSpawner();
#pragma endregion Overrides

#pragma region Component
protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	UArrowComponent* ArrowComponent = nullptr;
#pragma endregion Component

#pragma region Spawning
public:
	AFGAICharacter* SpawnAgent(TSubclassOf<AFGAICharacter> AgentClass);
#pragma endregion Spawning
};
