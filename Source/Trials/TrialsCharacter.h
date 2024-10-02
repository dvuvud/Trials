// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Trials/TrialsTypes/TurningInPlace.h"
#include "TrialsCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UWidgetComponent;
class AWeapon;
class UCombatComponent;
class UAnimMontage;

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking")
};

UCLASS()
class TRIALS_API ATrialsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATrialsCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostInitializeComponents() override;
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnhancedInput)
	UInputMappingContext* CharacterMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = EnhancedInput)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = EnhancedInput)
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = EnhancedInput)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = EnhancedInput)
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = EnhancedInput)
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = EnhancedInput)
	UInputAction* BlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = EnhancedInput)
	UInputAction* FlyAction;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = EnhancedInput)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = EnhancedInput)
	UInputAction* SheathAction;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = EnhancedInput)
	UInputAction* FlyUpDownAction;

	UPROPERTY(EditDefaultsOnly, Category = Animations)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animations)
	UAnimMontage* SheathMontage;


	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void BlockButtonPressed();
	void BlockButtonReleased();
	void AimOffset(float DeltaTime);
	void ToggleFlight();
	void LightAttackButtonPressed();
	void PlayMontageFunction(UAnimMontage* AttackMontage);
	void ToggleSheathStateButtonPressed();
	void FlyUpDownButtonPressed(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerToggleSheathStateButtonPressed();

	UFUNCTION(Server, Unreliable)
	void ServerFlyUpDown(float Value);

	int32 ToggleFlightInt = 1;

	UPROPERTY(EditAnywhere)
	float FlyUpDownSpeed = 10.f;

	UPROPERTY(ReplicatedUsing = OnRep_ToggleFlight)
	bool bIsFlying;

	UFUNCTION()
	void OnRep_ToggleFlight();

	virtual void Jump() override;

private:

	UPROPERTY(VisibleAnywhere, Category=Camera)
	USpringArmComponent* CameraSpringArm;
	UPROPERTY(VisibleAnywhere, Category=Camera)
	UCameraComponent* PlayerCamera;

	ETurningInPlace TurningInPlace;

	UPROPERTY(ReplicatedUsing = OnRep_ActionState, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Player States")
	EActionState ActionState = EActionState::EAS_Unoccupied;

	float AO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;


public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category="Steam")
	static FString GetSteamName(APlayerController* PlayerController);


private:

	APlayerController* TrialsPlayerController;

	UPROPERTY(EditAnywhere, Category = Camera)
	float MouseSensitivity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* PlayerNameWidget;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UPROPERTY(VisibleAnywhere)
	UCombatComponent* Combat;

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	UFUNCTION(Server, Reliable)
	void ServerToggleFlight();

	UFUNCTION()
	void OnRep_ActionState();

	UFUNCTION(Server, Reliable)
	void ServerLightAttack();

	bool RotationWasNotOrientedToMovement;
	bool WasUsingControllerRotationYaw;
	int32 AnimationSelectionINT;

	UPROPERTY(ReplicatedUsing = OnRep_SheathWeapon)
	bool bSheath;

	UFUNCTION()
	void OnRep_SheathWeapon();

public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsBlocking();
	AWeapon* GetEquippedWeapon();
	AWeapon* GetSheathedWeapon();
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE bool GetFlightState() const { return bIsFlying; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
};
