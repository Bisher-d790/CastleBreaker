#include "EnemyCharacter.h"

// Project
#include "CastleBreaker/AI/Enemy/EnemyAIController.h"


void AEnemyCharacter::HandlePawnSeen(APawn* Pawn)
{
	if (!IsValid(Pawn)) return;

	if (const auto AIController = GetController<AEnemyAIController>())
	{
		AIController->EnemyDetected(Pawn);
	}
}
