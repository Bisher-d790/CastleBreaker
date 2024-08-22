#pragma once
#include "GameFramework/PlayerController.h"

// Engine
#include "CoreMinimal.h"

#include "FGPlayerController.generated.h"

UCLASS()
class FIGHTINGGAME_API AFGPlayerController : public APlayerController
{
	GENERATED_BODY()

#pragma region Overrides
public:
	AFGPlayerController() = default;

protected:
	virtual void SetupInputComponent() override;
#pragma endregion Overrides

#pragma region Camera
protected:
	void TurnAtRate(const float Rate);

	void LookUpAtRate(const float Rate);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate = 45.f;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate = 45.f;
#pragma endregion Camera

#pragma region Input
protected:
	/** Called for jump input */
	void CharacterJump();

	void CharacterCrouchToggle();

	/** Called for forwards/backward input */
	void CharacterMoveForward(const float Value);

	/** Called for side to side input */
	void CharacterMoveRight(const float Value);
#pragma endregion Input
};
