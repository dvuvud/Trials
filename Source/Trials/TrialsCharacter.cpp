

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





ATrialsCharacter::ATrialsCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraSpringArm->SetupAttachment(GetMesh());
	CameraSpringArm->TargetArmLength = 400.f;
	CameraSpringArm->bUsePawnControlRotation = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	PlayerCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;


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

	if(GEngine)
	{
		FString FormattedMessage = FString::Printf(TEXT("Hello from %s!"), *GetSteamName(TrialsPlayerController));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FormattedMessage);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hello"));
	}
	
}

void ATrialsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrialsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
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

void ATrialsCharacter::Jump()
{
	Super::Jump();
}

FString ATrialsCharacter::GetSteamName(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
        if (OnlineSubsystem)
        {
            IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface();
            if (IdentityInterface.IsValid())
            {
                FUniqueNetIdRepl UniqueNetId = PlayerController->PlayerState->GetUniqueId();
                if (UniqueNetId.IsValid())
                {
                    FString PlayerName = IdentityInterface->GetPlayerNickname(*UniqueNetId);
                    return PlayerName;
                }
            }
        }
    }
    return FString("Unknown Player");
}