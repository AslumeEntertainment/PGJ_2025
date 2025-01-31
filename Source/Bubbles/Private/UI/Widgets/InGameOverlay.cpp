// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/InGameOverlay.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UInGameOverlay::SetTimerValue(int InTime)
{
	TXT_Timer->SetText(FText::FromString(FString::FromInt(InTime)));
}

void UInGameOverlay::SetCleanerScore(int Score)
{
	TXT_CleanerScore->SetText(FText::FromString(FString::FromInt(FMath::Abs(Score))));
}

void UInGameOverlay::SetContaminatorScore(int Score)
{
	TXT_ContaminatorScore->SetText(FText::FromString(FString::FromInt(FMath::Abs(Score))));
}

void UInGameOverlay::SetGameProgress(int GameProgress)
{
	GameProgressBar->SetPercent(GameProgress);
}
