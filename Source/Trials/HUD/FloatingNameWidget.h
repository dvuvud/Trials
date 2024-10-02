// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingNameWidget.generated.h"

/**
 * 
 */

class UTextBlock;

UCLASS()
class TRIALS_API UFloatingNameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public: 

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;

	UFUNCTION(BlueprintCallable)
	void SetDisplayText(FString TextToDisplay);

protected:
	virtual void NativeDestruct() override;

};
