#include "FGAICharacter.h"

// Engine
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Perception/PawnSensingComponent.h"


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