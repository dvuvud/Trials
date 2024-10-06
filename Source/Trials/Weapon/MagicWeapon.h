
#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "MagicWeapon.generated.h"

class AMagicProjectile;

UCLASS()
class TRIALS_API AMagicWeapon : public AWeapon
{
	GENERATED_BODY()

private:

public:
	virtual void BeginPlay() override;
	AMagicWeapon();
	virtual void ShootProjectile(const FVector& HitTarget) override;

private:
	virtual void OnRep_WeaponState() override;
	virtual void SetWeaponState(EWeaponState State) override; 
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UParticleSystemComponent* WeaponVFX;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMagicProjectile> ProjectileClass;
};
