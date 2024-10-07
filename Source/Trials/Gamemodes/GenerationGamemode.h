// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GenerationGamemode.generated.h"

/**
 * 
 */
class AVoxel;
class PerlinNoise;

UCLASS()
class TRIALS_API AGenerationGamemode : public AGameMode
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;

private:
	AVoxel* VoxelActor;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AVoxel> VoxelActorClass;
	void GenerateVoxelTerrain(PerlinNoise& NoiseGenerator, FVector Origin);
	// Constants for voxel size and noise parameters
	const int VOXEL_SIZE = 100;
	const int CHUNK_SIZE = 100; // Number of voxels along X and Y in one chunk
	const float SCALE = 0.0001f; // Controls noise frequency (Smaller values = smoother terrain)
	const float HEIGHT_MULTIPLIER = 5000.f;  // Controls how tall the terrain can be
};
