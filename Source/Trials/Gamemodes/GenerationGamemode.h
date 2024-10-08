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
	const int FREQUENCY = 2; 
	const int UNIT_SIZE = 9000;
	const float AMPLITUDE = 2000.f;  // Controls how tall the terrain can be
};
