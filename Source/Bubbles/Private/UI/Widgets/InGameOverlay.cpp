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

void UInGameOverlay::SetEffectivenessPercent(float EffectivenessPercent)
{
	if (EffectivenessPercent > 0)
	{
		EffectivenessProgressBar->SetFillColorAndOpacity(GameProgressBar->GetFillColorAndOpacity());
	}
	else
	{
		EffectivenessProgressBar->SetFillColorAndOpacity(TXT_ContaminatorScore->GetColorAndOpacity().GetSpecifiedColor());
	}
	EffectivenessProgressBar->SetPercent(FMath::Abs(EffectivenessPercent));
}

void UInGameOverlay::SetEnergyPercent(float EnergyPercent)
{
	EnergyProgressBar->SetPercent(EnergyPercent);
	TXT_UltimateIndicator->SetRenderOpacity(EnergyPercent == 1.f ? 1 : 0);
}

void UInGameOverlay::SetBubbleEffectivenessPercent(float BubbleEffectivenessPercent)
{
	if (BubbleEffectivenessPercent > 0)
	{
		BubbleEffectivenessProgressBar->SetFillColorAndOpacity(GameProgressBar->GetFillColorAndOpacity());
	}
	else
	{
		BubbleEffectivenessProgressBar->SetFillColorAndOpacity(TXT_ContaminatorScore->GetColorAndOpacity().GetSpecifiedColor());
	}
	BubbleEffectivenessProgressBar->SetPercent(FMath::Abs(BubbleEffectivenessPercent));
}

void UInGameOverlay::SetBubbleEffectivenessVisibility(bool bIsVisible)
{
	BubbleEffectivenessProgressBar->SetRenderOpacity(bIsVisible ? 1.f : 0.f);
	TXT_BubbleEffectiveness->SetRenderOpacity(bIsVisible ? 1.f : 0.f);
}

