// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trials/TrialsTypes/WeaponType.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	EWS_Unequipped UMETA(DisplayName = "Unequipped"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX"),
};

class USphereComponent;
class UWidgetComponent;
class UBoxComponent;

UCLASS()
class TRIALS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void ShowPickupWidget(bool bShowWidget);

	TArray<AActor*> IgnoreActors;

	/*
	* Textures for weapon crosshairs
	*/

	UPROPERTY(EditAnywhere)
	UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere)
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere)
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere)
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere)
	UTexture2D* CrosshairsBottom;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	EWeaponType WeaponType;

	UFUNCTION()
	virtual void OnRep_WeaponState();

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	UFUNCTION(BlueprintCallable)
	virtual void ShootProjectile(const FVector& HitTarget);

private:
	

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USphereComponent* AreaSphere;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UWidgetComponent* PickupWidget;

	UFUNCTION(BlueprintCallable)
	float FloatingSin(float DeltaTime, float Speed, float Altitude);

	float HoverSpeed = 0.f;

	
public:	
	
	virtual void SetWeaponState(EWeaponState State);
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

};
