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
	

private:
	void GenerateNoiseMap();
	void GenerateWorld();
	void GenerateRegions();
	void SpawnVoxel(const FVector& Location);
	void UnloadVoxel(const FVector& Location);
	UPROPERTY(EditAnywhere, Category = "Voxel")
	TSubclassOf<AActor> VoxelBlueprint;
};
