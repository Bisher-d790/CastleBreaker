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
