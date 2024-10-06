
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Trials/Interfaces/GetHitInterface.h"
#include "Enemy.generated.h"

class UAnimInstace;

UCLASS()
class TRIALS_API AEnemy : public ACharacter, public IGetHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void GetHit(const FVector& ImpactPoint) override;

	virtual void GetHitMagic(const FVector& ImpactPoint) override;

	void DirectionalHitReact(const FVector& ImpactPoint);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


private:

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(ReplicatedUsing = OnRep_Section)
	FName Section = FName("PlaceHolder");

	UFUNCTION()
	void OnRep_Section();

	UPROPERTY(ReplicatedUsing = OnRep_Section)
	int32 ForceReplicationInt;

	FName PrevSection = FName("PlaceHolder");

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VFX)
	UParticleSystem* HitParticles;

protected:

	virtual void BeginPlay() override;


	void PlayHitReact(const FName& SectionName);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
