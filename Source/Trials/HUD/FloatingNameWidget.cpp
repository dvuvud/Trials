// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingNameWidget.h"
#include "Components/TextBlock.h"


void UFloatingNameWidget::NativeDestruct()
{
    RemoveFromParent();
    Super::NativeDestruct();
}

void UFloatingNameWidget::SetDisplayText(FString TextToDisplay)
{
    if (PlayerNameText)
    {
        PlayerNameText->SetText(FText::FromString(TextToDisplay));
    }

}
