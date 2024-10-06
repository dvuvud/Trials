// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GenerationGamemode.generated.h"

/**
 * 
 */
UCLASS()
class TRIALS_API AGenerationGamemode : public AGameMode
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Voxel")
	TSubclassOf<AActor> VoxelBlueprint;
	
};
