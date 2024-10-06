

#include "MagicWeapon.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "MagicProjectile.h"

AMagicWeapon::AMagicWeapon()
{
	WeaponVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponVFX"));
	WeaponVFX->SetupAttachment(RootComponent);
}

void AMagicWeapon::ShootProjectile(const FVector& HitTarget)
{
	Super::ShootProjectile(HitTarget);

	if (!HasAuthority()) return;
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* ProjectileSocket = GetWeaponMesh()->GetSocketByName(FName("ProjectileSocket"));
	if (ProjectileSocket)
	{
		FTransform SocketTransform = ProjectileSocket->GetSocketTransform(GetWeaponMesh());
		FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();
		if (ProjectileClass && InstigatorPawn)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = InstigatorPawn;
			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<AMagicProjectile>(
					ProjectileClass,
					SocketTransform.GetLocation(),
					TargetRotation,
					SpawnParams
				);
			}
		}
	}
}

void AMagicWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void AMagicWeapon::OnRep_WeaponState()
{
	Super::OnRep_WeaponState();
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		WeaponType = EWeaponType::EWT_Magic;
		break;
	case EWeaponState::EWS_Unequipped:
		break;
	}
}

void AMagicWeapon::SetWeaponState(EWeaponState State)
{
	Super::SetWeaponState(State);
	WeaponState = State;
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		WeaponType = EWeaponType::EWT_Magic;
		WeaponMesh->SetVisibility(true);
		break;
	case EWeaponState::EWS_Unequipped:
		break;
	}
}
