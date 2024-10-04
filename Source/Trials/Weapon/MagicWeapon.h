
#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "MagicWeapon.generated.h"

UCLASS()
class TRIALS_API AMagicWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	AMagicWeapon();

private:
	virtual void OnRep_WeaponState() override;
	virtual void SetWeaponState(EWeaponState State) override; 
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UParticleSystemComponent* WeaponVFX;
};
