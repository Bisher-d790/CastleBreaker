#include "EnemyCharacter.h"

// Project
#include "FightingGame/AI/Enemy/EnemyAIController.h"


void AEnemyCharacter::HandlePawnSeen(APawn* Pawn)
{
	if (!IsValid(Pawn)) return;

	if (const auto AIController = GetController<AEnemyAIController>())
	{
		AIController->EnemyDetected(Pawn);
	}
}
