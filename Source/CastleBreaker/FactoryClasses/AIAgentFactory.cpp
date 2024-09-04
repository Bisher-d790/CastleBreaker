#include "AIAgentFactory.h"

// Engine
#include "Components/ArrowComponent.h"

// Project
#include "CastleBreaker/AI/CBAICharacter.h"


AAIAgentFactory::AAIAgentFactory()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);
}

ACBAICharacter* AAIAgentFactory::SpawnAgent(TSubclassOf<ACBAICharacter> AgentClass)
{
	const auto World = GetWorld();
	if (!IsValid(World)) return nullptr;

	const auto SpawnedAgent = World->SpawnActor<ACBAICharacter>(AgentClass, ArrowComponent->GetComponentTransform());

	return SpawnedAgent;
}
