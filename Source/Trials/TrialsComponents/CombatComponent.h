
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


#define TRACE_LENGTH 80000.f

class AWeapon;
class ATrialsCharacter;
class ATrialsHUD;
class ATrialsPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRIALS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	friend class ATrialsCharacter;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	void EquipWeapon(AWeapon* WeaponToEquip);

	UFUNCTION(BlueprintCallable)
	void AttachWeapon(AWeapon* WeaponToAttach, FName SocketName);

	UFUNCTION(BlueprintCallable)
	void DetachWeapon(AWeapon* WeaponToAttach, FName SocketName);

	void ToggleSheathState(AWeapon* WeaponToSheath);

	void ShootProjectileNotifyBegan();
	
protected:
	virtual void BeginPlay() override;

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetBlocking(bool bIsBlocking);

	UFUNCTION(Server, Reliable)
	void ServerSetBlocking(bool bIsBlocking);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	void SetHUDCrosshairs(float DeltaTime);

private:
	ATrialsCharacter* Character;
	ATrialsHUD* HUD;
	ATrialsPlayerController* TrialsPlayerController;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	AWeapon* SheathedWeapon;

	UPROPERTY(Replicated)
	bool bBlocking;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float BlockWalkSpeed;

	UFUNCTION(Server, Reliable)
	void ServerShootProjectileNotifyBegan(const FVector HitTarget);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
