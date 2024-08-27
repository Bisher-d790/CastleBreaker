#include "AIAgentSpawner.h"

// Engine
#include "Components/ArrowComponent.h"

// Project
#include "FightingGame/AI/FGAICharacter.h"


AAIAgentSpawner::AAIAgentSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);
}

AFGAICharacter* AAIAgentSpawner::SpawnAgent(TSubclassOf<AFGAICharacter> AgentClass)
{
	const auto World = GetWorld();
	if (!IsValid(World)) return nullptr;

	const auto SpawnedAgent = World->SpawnActor<AFGAICharacter>(AgentClass, ArrowComponent->GetComponentTransform());

	return SpawnedAgent;
}
