

#include "TrialsCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Trials/Weapon/Weapon.h"
#include "Trials/TrialsComponents/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimMontage.h"
#include "Trials/HUD/FloatingNameWidget.h"



ATrialsCharacter::ATrialsCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraSpringArm->SetupAttachment(GetMesh());
	CameraSpringArm->TargetArmLength = 400.f;
	CameraSpringArm->bUsePawnControlRotation = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	PlayerCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 600.f);

	PlayerNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FloatingNameWidget"));
	PlayerNameWidget->SetupAttachment(RootComponent);

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);


	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
}

void ATrialsCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ATrialsCharacter, OverlappingWeapon, COND_OwnerOnly);
	DOREPLIFETIME(ATrialsCharacter, bIsFlying);
	DOREPLIFETIME(ATrialsCharacter, ActionState);
	DOREPLIFETIME(ATrialsCharacter, bSheath);
	DOREPLIFETIME(ATrialsCharacter, ReplicatedPlayerName);
}

void ATrialsCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Combat)
	{
		Combat->Character = this;
	}
}

void ATrialsCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	AWeapon* EquippedWeapon = GetEquippedWeapon();
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void ATrialsCharacter::BeginPlay()
{
	Super::BeginPlay();

	TrialsPlayerController = Cast<APlayerController>(GetController());
	if (TrialsPlayerController)
	{
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(TrialsPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
	}

	if (HasAuthority())
	{
		MulticastPlayerName(GetSteamName());
	}
	else if (IsLocallyControlled())
	{
		MulticastPlayerName(GetSteamName());
	}
	else if (!HasAuthority() && !IsLocallyControlled())
	{
		ServerGetPlayerName();
	}
}

void ATrialsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime);

}

void ATrialsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ThisClass::EquipButtonPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::CrouchButtonPressed);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &ThisClass::BlockButtonPressed);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &ThisClass::BlockButtonReleased);
		EnhancedInputComponent->BindAction(FlyAction, ETriggerEvent::Completed, this, &ThisClass::ToggleFlight);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &ThisClass::LightAttackButtonPressed);
		EnhancedInputComponent->BindAction(SheathAction, ETriggerEvent::Started, this, &ThisClass::ToggleSheathStateButtonPressed);
		EnhancedInputComponent->BindAction(FlyUpDownAction, ETriggerEvent::Triggered, this, &ThisClass::FlyUpDownButtonPressed);
	}
}

void ATrialsCharacter::Move(const FInputActionValue& Value)
{
	if (Controller)
	{
		const FVector2D UnitVector = Value.Get<FVector2D>();
		const FRotator UnitRotation(0, Controller->GetControlRotation().Yaw, 0);
		if (UnitVector.Y != 0.f)
		{
			const FVector Direction = UnitRotation.RotateVector(FVector::ForwardVector);

			AddMovementInput(Direction, UnitVector.Y);
		}

		if (UnitVector.X != 0.f)
		{
			const FVector Direction = UnitRotation.RotateVector(FVector::RightVector);
	
			AddMovementInput(Direction, UnitVector.X);
		}
	}
}

void ATrialsCharacter::Look(const FInputActionValue& Value)
{
	if (Controller)
	{
		const FVector2D LookValue = Value.Get<FVector2D>();
		if (LookValue.X != 0.f)
		{
			AddControllerYawInput(LookValue.X * MouseSensitivity);
		}
		if (LookValue.Y != 0.f)
		{
			AddControllerPitchInput(LookValue.Y * MouseSensitivity);
		}
	}
}

void ATrialsCharacter::EquipButtonPressed()
{
	if (GetEquippedWeapon() || GetSheathedWeapon()) { return; }
	if (Combat)
	{
		if (HasAuthority())
		{
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			ServerEquipButtonPressed();
		}
	}
}

void ATrialsCharacter::CrouchButtonPressed()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else 
	{
		Crouch();
	}
	
}

void ATrialsCharacter::BlockButtonPressed()
{
	if (bIsFlying || ActionState == EActionState::EAS_Attacking) return;
	if (GetEquippedWeapon() == nullptr) return;
	if (Combat)
	{
		Combat->SetBlocking(true);
	}
}

void ATrialsCharacter::BlockButtonReleased()
{
	if (Combat && Combat->bBlocking)
	{
		Combat->SetBlocking(false);
	}
}

void ATrialsCharacter::AimOffset(float DeltaTime)
{
	if (Combat && Combat->EquippedWeapon == nullptr) return;
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = GetCharacterMovement()->GetCurrentAcceleration().Size();
	bool bIsInAir = GetCharacterMovement()->IsFalling();
	if (Speed == 0.f && !bIsInAir)
	{
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
	}
	if (Speed > 0.f || bIsInAir)
	{
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}
	AO_Pitch = GetBaseAimRotation().Pitch;
	
}

void ATrialsCharacter::ToggleFlight()
{

	if (HasAuthority())
	{
		ToggleFlightInt++;
		if (ToggleFlightInt % 2 == 0)
		{
			if (!GetCharacterMovement()) { return; }
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			bIsFlying = true;
		}
		else 
		{
			if (!GetCharacterMovement()) { return; }
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			bIsFlying = false;
		}
	}
	else
	{
		ServerToggleFlight();
	}
}

void ATrialsCharacter::PlayMontageFunction(UAnimMontage* MontageToPlay)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if (AnimInstance && MontageToPlay)
	{
		AnimInstance->Montage_Play(MontageToPlay);
		AnimationSelectionINT = (AnimationSelectionINT + 1) % 2;
		FName SelectionName = FName();
		switch (AnimationSelectionINT)
		{
		case 0:
			SelectionName = FName("Attack1");
				break;
		case 1:
			SelectionName = FName("Attack2");
				break;
		}
		AnimInstance->Montage_JumpToSection(SelectionName, MontageToPlay);
	}
}

void ATrialsCharacter::ToggleSheathStateButtonPressed()
{
	if (ActionState == EActionState::EAS_Attacking) { return; }
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AWeapon* EquippedWeapon = GetEquippedWeapon();
	if (Combat && EquippedWeapon && HasAuthority() && SheathMontage && AnimInstance)
	{
		Combat->ToggleSheathState(EquippedWeapon);
		bSheath = true;
		AnimInstance->Montage_Play(SheathMontage);
		AnimInstance->Montage_JumpToSection(FName("Sheath"), SheathMontage);
	}
	else if (Combat && EquippedWeapon)
	{
		ServerToggleSheathStateButtonPressed();
	}
	else if (Combat && GetSheathedWeapon() && HasAuthority() && SheathMontage && AnimInstance)
	{
		Combat->ToggleSheathState(GetSheathedWeapon());
		bSheath = false;
		AnimInstance->Montage_Play(SheathMontage);
		AnimInstance->Montage_JumpToSection(FName("Equip"), SheathMontage);
		
	}
	else if (Combat && GetSheathedWeapon())
	{
		ServerToggleSheathStateButtonPressed();
	}
}

void ATrialsCharacter::FlyUpDownButtonPressed(const FInputActionValue& Value)
{
	if (bIsFlying && HasAuthority())
	{
		FVector CurrentLocation = GetActorLocation();
		FVector DirectionVector = GetActorUpVector();
		FVector NewVector = CurrentLocation + DirectionVector * Value.Get<float>() * FlyUpDownSpeed;
		SetActorLocation(NewVector);
	}
	else if (bIsFlying && IsLocallyControlled())
	{
		ServerFlyUpDown(Value.Get<float>());
		FVector CurrentLocation = GetActorLocation();
		FVector DirectionVector = GetActorUpVector();
		FVector NewVector = CurrentLocation + DirectionVector * Value.Get<float>() * FlyUpDownSpeed;
		SetActorLocation(NewVector);
	}
}

void ATrialsCharacter::ServerFlyUpDown_Implementation(float Value)
{
	FVector CurrentLocation = GetActorLocation();
	FVector DirectionVector = GetActorUpVector();
	FVector NewVector = CurrentLocation + DirectionVector * Value * FlyUpDownSpeed;
	SetActorLocation(NewVector);
}

void ATrialsCharacter::ServerToggleSheathStateButtonPressed_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (Combat && GetEquippedWeapon() && SheathMontage && AnimInstance)
	{
		Combat->ToggleSheathState(GetEquippedWeapon());
		bSheath = true;
		AnimInstance->Montage_Play(SheathMontage);
		AnimInstance->Montage_JumpToSection(FName("Sheath"), SheathMontage);
	}
	else if (Combat && GetSheathedWeapon() && SheathMontage && AnimInstance)
	{
		Combat->ToggleSheathState(GetSheathedWeapon());
		bSheath = false;
		AnimInstance->Montage_Play(SheathMontage);
		AnimInstance->Montage_JumpToSection(FName("Equip"), SheathMontage);
	}
}

void ATrialsCharacter::OnRep_ToggleFlight()
{
	ToggleFlightInt++;
	if (ToggleFlightInt % 2 == 0)
	{
		if (!GetCharacterMovement()) { return; }
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);		
		bIsFlying = true;
	}
	else
	{
		if (!GetCharacterMovement()) { return; }
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		bIsFlying = false;
	}
}

void ATrialsCharacter::Jump()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Super::Jump();
	}
}

FString ATrialsCharacter::GetSteamName()
{
	FString PlayerName;
	if (TrialsPlayerController)
	{
		IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
		if (OnlineSubsystem)
		{
			IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface();
			if (IdentityInterface.IsValid())
			{
				APlayerState* SteamPlayerState = TrialsPlayerController->PlayerState;
				if (SteamPlayerState)
				{
					FUniqueNetIdRepl UniqueNetId = SteamPlayerState->GetUniqueId();
					if (UniqueNetId.IsValid())
					{
						PlayerName = IdentityInterface->GetPlayerNickname(*UniqueNetId);
					}
				}
			}
		}
	}
	if (PlayerName.Len() == 0)
	{
		PlayerName = FString::Printf(TEXT("Unknown Player"));
	}

	return PlayerName;
}

void ATrialsCharacter::MulticastPlayerName_Implementation(const FString& PlayerName)
{
	if (PlayerNameWidget)
	{
		UFloatingNameWidget* FloatingNameWidget = Cast<UFloatingNameWidget>(PlayerNameWidget->GetUserWidgetObject());
		if (FloatingNameWidget)
		{
			FloatingNameWidget->SetDisplayText(PlayerName);
		}
	}
}

void ATrialsCharacter::OnRep_PlayerName()
{
	if (PlayerNameWidget)
	{
		UFloatingNameWidget* FloatingNameWidget = Cast<UFloatingNameWidget>(PlayerNameWidget->GetUserWidgetObject());
		if (FloatingNameWidget)
		{
			FloatingNameWidget->SetDisplayText(ReplicatedPlayerName);
		}
	}
}

void ATrialsCharacter::ServerGetPlayerName_Implementation()
{
	ReplicatedPlayerName = GetSteamName();
}

void ATrialsCharacter::SetActionState(EActionState NewActionState)
{
	ActionState = NewActionState;
}

void ATrialsCharacter::ServerEquipButtonPressed_Implementation()
{
	if (Combat)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

void ATrialsCharacter::ServerToggleFlight_Implementation()
{
	ToggleFlightInt++;
	if (ToggleFlightInt % 2 == 0)
	{
		if (!GetCharacterMovement()) { return; }
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		bIsFlying = true;
	}
	else
	{
		if (!GetCharacterMovement()) { return; }
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		bIsFlying = false;
	}
}

void ATrialsCharacter::OnRep_SheathWeapon()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (bSheath && SheathMontage && AnimInstance)
	{
		AnimInstance->Montage_Play(SheathMontage);
		AnimInstance->Montage_JumpToSection(FName("Sheath"), SheathMontage);
	}
	else if (!bSheath && SheathMontage && AnimInstance)
	{
		AnimInstance->Montage_Play(SheathMontage);
		AnimInstance->Montage_JumpToSection(FName("Equip"), SheathMontage);
	}
}

void ATrialsCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon && IsLocallyControlled())
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = Weapon;
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

bool ATrialsCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

bool ATrialsCharacter::IsBlocking()
{
	return (Combat && Combat->bBlocking);
}

AWeapon* ATrialsCharacter::GetEquippedWeapon()
{
	if (Combat == nullptr) return nullptr;
	return Combat->EquippedWeapon;
}

AWeapon* ATrialsCharacter::GetSheathedWeapon()
{
	if (Combat == nullptr) return nullptr;
	return Combat->SheathedWeapon;
}

void ATrialsCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

void ATrialsCharacter::MulticastLightAttack_Implementation()
{
	AWeapon* EquippedWeapon = GetEquippedWeapon();
	if (!EquippedWeapon) { return; }
	switch (EquippedWeapon->GetWeaponType())
	{
	case EWeaponType::EWT_Melee:
		if (AttackMontage)
		{
			PlayMontageFunction(AttackMontage);
			ActionState = EActionState::EAS_Attacking;
		}
		break;
	case EWeaponType::EWT_Magic:
		if (MagicAttackMontage)
		{
			PlayMontageFunction(MagicAttackMontage);
			ActionState = EActionState::EAS_Attacking;
		}
		break;
	default:
		break;
	}
}

void ATrialsCharacter::ServerLightAttack_Implementation()
{
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		ActionState = EActionState::EAS_Attacking;
		MulticastLightAttack();
	}
}

void ATrialsCharacter::LightAttackButtonPressed()
{
	if (!GetEquippedWeapon()) return;
	if (!HasAuthority() && Combat && ActionState == EActionState::EAS_Unoccupied)
	{
		ServerLightAttack();
	}
	else if (HasAuthority() && Combat && ActionState == EActionState::EAS_Unoccupied)
	{
		MulticastLightAttack();
	}
}