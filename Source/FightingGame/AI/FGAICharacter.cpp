#include "FGAICharacter.h"

// Engine
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Perception/PawnSensingComponent.h"

// Project
#include "FightingGame/AI/FGAIController.h"


AFGAICharacter::AFGAICharacter()
{
	// Remove unnecessary components
	CameraBoom->DestroyComponent();
	FollowCamera->DestroyComponent();

	// Setup components
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
}

void AFGAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(PawnSensingComponent))
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AFGAICharacter::HandlePawnSeen);
	}
}

void AFGAICharacter::HandlePawnSeen(APawn* Pawn)
{
	if (!IsValid(Pawn)) return;

	if (const auto AIC = GetController<AFGAIController>())
	{
		AIC->EnemyDetected(Pawn);
	}
}
