// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TrialsPlayerController.generated.h"

/**
 * 
 */

class AGenerationGamemode;

UCLASS()
class TRIALS_API ATrialsPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:

	UFUNCTION(Server, Reliable)
	void ServerLoadChunks(FVector2D ChunkOrigin);

	FVector PlayerLocation;

	AGenerationGamemode* Gamemode;

	TSet<FIntVector> LoadedChunks;
	int32 VOXELSIZE = 100;
	const int32 CHUNK_SIZE = 16;
	const int32 LOAD_DISTANCE = 1;
};
