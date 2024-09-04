#include "CBAICharacter.h"

// Engine
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "NavigationSystem/Public/NavigationInvokerComponent.h"


ACBAICharacter::ACBAICharacter()
{
	// Remove unnecessary components
	CameraBoom->DestroyComponent();
	FollowCamera->DestroyComponent();

	// Setup components
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	NavigationInvokerComponent = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvoker"));
}

void ACBAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(PawnSensingComponent))
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &ACBAICharacter::HandlePawnSeen);
	}
}