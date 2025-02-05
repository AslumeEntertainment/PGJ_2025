// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/LoadingScreen.h"

#include "Components/TextBlock.h"

void ULoadingScreen::SetLoadingText(FText LoadingText)
{
	TXT_LoadingText->SetText(LoadingText);
}
