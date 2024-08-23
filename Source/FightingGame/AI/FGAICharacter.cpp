#include "FGAICharacter.h"

// Engine
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


AFGAICharacter::AFGAICharacter()
{
	// Remove unnecessary components
	CameraBoom->DestroyComponent();
	FollowCamera->DestroyComponent();
}

void AFGAICharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateHealthBar();
}

void AFGAICharacter::TakeDamage(const float Damage)
{
	Super::TakeDamage(Damage);

	UpdateHealthBar();
}

void AFGAICharacter::Heal(const float HealAmount)
{
	Super::Heal(HealAmount);

	UpdateHealthBar();
}
