// Fill out your copyright notice in the Description page of Project Settings.


#include "TrialsAnimInstance.h"
#include "TrialsCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Trials/TrialsComponents/CombatComponent.h"

void UTrialsAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    TrialsCharacter = Cast<ATrialsCharacter>(TryGetPawnOwner());
    
}

void UTrialsAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (Speed == 0)
    {
        IdlingTime += DeltaTime;
    }
    if (Speed > 0)
    {
        IdlingTime = 0;
    }

    if (TrialsCharacter == nullptr)
    {
        TrialsCharacter = Cast<ATrialsCharacter>(TryGetPawnOwner());
    }
    if (!TrialsCharacter)return;

    FVector Velocity = TrialsCharacter->GetVelocity();
    Velocity.Z = 0.f;
    Speed = Velocity.Size();

    bIsInAir = TrialsCharacter->GetCharacterMovement()->IsFalling();

    bIsAccelerating = TrialsCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
    bWeaponEquipped = TrialsCharacter->IsWeaponEquipped();
    EquippedWeapon = TrialsCharacter->GetEquippedWeapon();
    bIsCrouched = TrialsCharacter->bIsCrouched;
    bBlocking = TrialsCharacter->IsBlocking();
    TurningInPlace = TrialsCharacter->GetTurningInPlace();

    AO_Yaw = TrialsCharacter->GetAO_Yaw();
    AO_Pitch = TrialsCharacter->GetAO_Pitch();
    FRotator AimRotation = TrialsCharacter->GetBaseAimRotation();
    FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(TrialsCharacter->GetVelocity());
    FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
    DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
    YawOffset = DeltaRotation.Yaw;
    bIsFlying = TrialsCharacter->GetFlightState();
    if (EquippedWeapon)
    {
        WeaponState = EquippedWeapon->GetWeaponState();
        WeaponType = EquippedWeapon->GetWeaponType();
    }
    SheathedWeapon = TrialsCharacter->GetSheathedWeapon();
    CombatComponent = TrialsCharacter->GetCombat();


    if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && TrialsCharacter->GetMesh())
    {
        LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
        FVector OutPosition;
        FRotator OutRotation;
        TrialsCharacter->GetMesh()->TransformToBoneSpace(FName("RightHand"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
        LeftHandTransform.SetLocation(OutPosition);
        LeftHandTransform.SetRotation(FQuat(OutRotation));
    }
}

void UTrialsAnimInstance::SetIdlingTime(float NewIdlingTime)
{
    IdlingTime = NewIdlingTime;
}

void UTrialsAnimInstance::OnShootProjectileNotify()
{
    if (CombatComponent)
    {
        CombatComponent->ShootProjectileNotifyBegan();
    }
}
