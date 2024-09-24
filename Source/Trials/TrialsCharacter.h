// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TrialsCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class TRIALS_API ATrialsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATrialsCharacter();

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

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);


	virtual void Jump() override;

private:

UPROPERTY(VisibleAnywhere, Category=Camera)
USpringArmComponent* CameraSpringArm;
UPROPERTY(VisibleAnywhere, Category=Camera)
UCameraComponent* PlayerCamera;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category="Steam")
	static FString GetSteamName(APlayerController* PlayerController);

private:

	APlayerController* TrialsPlayerController;

	UPROPERTY(EditAnywhere, Category = Camera)
	float MouseSensitivity = 0.5f;


};
