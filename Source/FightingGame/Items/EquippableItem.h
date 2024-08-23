#pragma once
#include "GameFramework/Actor.h"

// Engine
#include "CoreMinimal.h"

#include "EquippableItem.generated.h"

class UMeshComponent;
class USphereComponent;

/// <summary>
/// Base class for any item that could be equipped,
/// Usually has a Primary and Secondary actions to execute
/// </summary>
UCLASS(Blueprintable)
class AEquippableItem : public AActor
{
	GENERATED_BODY()

#pragma region Overrides
public:
	AEquippableItem();

protected:
	virtual void Tick(float DeltaTime) override;
#pragma endregion Overrides

#pragma region Components
public:
	UFUNCTION(BlueprintPure, Category = "Components")
	FORCEINLINE UMeshComponent* GetMesh() const { return Mesh; }

	// Could be used for Pick up functionality later on
	UFUNCTION(BlueprintPure, Category = "Components")
	FORCEINLINE USphereComponent* GetCollision() const { return Collision; }

protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	UMeshComponent* Mesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* Collision = nullptr;
#pragma endregion Components

#pragma region Actions
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAction, const bool, bHasStarted);

	UFUNCTION(BlueprintCallable, Category = "PrimaryAction")
	virtual void StartPrimaryAction();

	UFUNCTION(BlueprintCallable, Category = "PrimaryAction")
	virtual void StopPrimaryAction();

	UFUNCTION(BlueprintPure, Category = "PrimaryAction")
	FORCEINLINE bool IsPrimaryActionActive() const { return bIsPrimaryActionActive; }

	UFUNCTION(BlueprintPure, Category = "PrimaryAction")
	FORCEINLINE float GetPrimaryActionTimeActive() const { return PrimaryActionTimeActive; }

	UPROPERTY(BlueprintAssignable, Category = "PrimaryAction")
	FOnAction OnPrimaryAction;

	UFUNCTION(BlueprintCallable, Category = "SecondaryAction")
	virtual void StartSecondaryAction();

	UFUNCTION(BlueprintCallable, Category = "SecondaryAction")
	virtual void StopSecondaryAction();

	UFUNCTION(BlueprintPure, Category = "SecondaryAction")
	FORCEINLINE bool IsSecondaryActionActive() const { return bIsSecondaryActionActive; }

	UFUNCTION(BlueprintPure, Category = "SecondaryAction")
	FORCEINLINE float GetSecondaryActionTimeActive() const { return SecondaryActionTimeActive; }

	UPROPERTY(BlueprintAssignable, Category = "SecondaryAction")
	FOnAction OnSecondaryAction;

protected:
	UPROPERTY() bool bIsPrimaryActionActive = false;
	UPROPERTY() float PrimaryActionTimeActive = 0.f;

	UPROPERTY() bool bIsSecondaryActionActive = false;
	UPROPERTY() float SecondaryActionTimeActive = 0.f;
#pragma endregion Actions
};