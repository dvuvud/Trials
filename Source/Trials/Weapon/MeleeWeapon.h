
#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */


UCLASS()
class TRIALS_API AMeleeWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	AMeleeWeapon();
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	virtual void OnBoxOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USceneComponent* BoxTraceStart;
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USceneComponent* BoxTraceEnd;

	virtual void OnRep_WeaponState() override;
	virtual void SetWeaponState(EWeaponState State) override;
public:
};
