// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/InGameOverlay.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UInGameOverlay::SetTimerValue(int InTime)
{
	FString Time = FString::FromInt(InTime / 60);

	if (InTime % 60 < 10) Time.Append(":0");
	else Time.Append(":");

	Time.Append(FString::FromInt(InTime % 60));

	TXT_Timer->SetText(FText::FromString(Time));
}

void UInGameOverlay::SetCleanerScore(int Score)
{
	TXT_CleanerScore->SetText(FText::FromString(FString::FromInt(FMath::Abs(Score))));
}

void UInGameOverlay::SetContaminatorScore(int Score)
{
	TXT_ContaminatorScore->SetText(FText::FromString(FString::FromInt(FMath::Abs(Score))));
}

void UInGameOverlay::SetGameProgress(float GameProgress)
{
	GameProgressBar->SetPercent(GameProgress);
}
