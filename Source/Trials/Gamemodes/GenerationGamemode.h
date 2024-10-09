// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GenerationGamemode.generated.h"

/**
 * 
 */
class AVoxel;

UCLASS()
class TRIALS_API AGenerationGamemode : public AGameMode
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;

	void GenerateVoxelTerrain(FVector2D Origin);

private:
	AVoxel* ChunkVoxelActor;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AVoxel> VoxelActorClass;
	// Constants for voxel size and noise parameters
	const int VOXEL_SIZE = 100;
	const int CHUNK_SIZE = 16; // Number of voxels along X and Y in one chunk
	const int FREQUENCY = 2; 
	const int UNIT_SIZE = 15000;
	const int AMPLITUDE = 2000;  // Controls how tall the terrain can be
};
