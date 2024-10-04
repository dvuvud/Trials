

#include "MagicWeapon.h"
#include "Particles/ParticleSystemComponent.h"


void AMagicWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponMesh->SetVisibility(false);
}

AMagicWeapon::AMagicWeapon()
{
	WeaponVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponVFX"));
	WeaponVFX->SetupAttachment(RootComponent);
}

void AMagicWeapon::OnRep_WeaponState()
{
	Super::OnRep_WeaponState();
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		WeaponType = EWeaponType::EWT_Magic;
		if (WeaponVFX)
		{
			WeaponVFX->DeactivateSystem();
		}
		WeaponMesh->SetVisibility(true);
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
		if (WeaponVFX)
		{
			WeaponVFX->DeactivateSystem();
		}
		WeaponMesh->SetVisibility(true);
		break;
	case EWeaponState::EWS_Unequipped:
		break;
	}
}
