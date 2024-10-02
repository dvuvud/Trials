// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGamemode.generated.h"

/**
 * 
 */
UCLASS()
class TRIALS_API ALobbyGamemode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer);

	UFUNCTION(BlueprintCallable)
	void MoveToEureka();

	UPROPERTY(VisibleAnywhere)
	int32 NumberOfPlayers;

};
