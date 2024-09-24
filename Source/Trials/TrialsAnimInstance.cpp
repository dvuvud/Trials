// Fill out your copyright notice in the Description page of Project Settings.


#include "TrialsAnimInstance.h"
#include "TrialsCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTrialsAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    TrialsCharacter = Cast<ATrialsCharacter>(TryGetPawnOwner());
}

void UTrialsAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

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
}