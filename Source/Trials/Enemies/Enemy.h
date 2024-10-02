
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Trials/Interfaces/GetHitInterface.h"
#include "Enemy.generated.h"

UCLASS()
class TRIALS_API AEnemy : public ACharacter, public IGetHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void GetHit(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
