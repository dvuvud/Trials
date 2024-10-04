// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicProjectile.generated.h"

class UBoxComponent;

UCLASS()
class TRIALS_API AMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagicProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionBox;

};
