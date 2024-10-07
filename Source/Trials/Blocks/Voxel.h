// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Voxel.generated.h"

class UInstancedStaticMeshComponent;

UCLASS()
class TRIALS_API AVoxel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVoxel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UInstancedStaticMeshComponent* InstancedMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE UInstancedStaticMeshComponent* GetISM() { return InstancedMeshComponent; }
};
