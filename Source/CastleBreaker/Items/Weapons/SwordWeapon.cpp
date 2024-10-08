#include "SwordWeapon.h"

// Engine
#include "Components/CapsuleComponent.h"

// Project
#include "CastleBreaker/Interfaces/DamageableInterface.h"
#include "CastleBreaker/Player/CBPlayerController.h"


ASwordWeapon::ASwordWeapon()
{
	BladeCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BladeCollision"));
	BladeCollision->SetupAttachment(RootComponent);
	BladeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASwordWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(BladeCollision))
	{
		BladeCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleBladeBeginOverlap);
	}
}

void ASwordWeapon::StartAttack()
{
	Super::StartAttack();

	if (IsValid(BladeCollision))
		BladeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASwordWeapon::FinishAttack()
{
	Super::FinishAttack();

	if (IsValid(BladeCollision))
		BladeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AttackedActors.Empty();
}

void ASwordWeapon::HandleBladeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor)
		|| OtherActor == GetOwner()
		|| AttackedActors.Contains(OtherActor)) return;



	if (const auto DamageableActor = Cast<IDamageableInterface>(OtherActor))
	{
		if (!CanDamageActor(OtherActor)) return;

		DamageableActor->TakeDamage(AttackDamage);

		AttackedActors.Add(OtherActor);

		// If damaged actor is killed
		if (DamageableActor->GetHealth() <= 0.f && GetOwner())
		{
			if (const auto OwnerPC = GetOwner()->GetInstigatorController<ACBPlayerController>())
				OwnerPC->EnemyKilled(OtherActor);
		}
	}
}
