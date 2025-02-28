// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameOverlay.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class BUBBLES_API UInGameOverlay : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TXT_Timer;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TXT_CleanerScore;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TXT_ContaminatorScore;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TXT_BubbleEffectiveness;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TXT_UltimateIndicator;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* GameProgressBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* EffectivenessProgressBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* EnergyProgressBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* BubbleEffectivenessProgressBar;

public:

	UFUNCTION()
	void SetTimerValue(int InTime);

	UFUNCTION()
	void SetCleanerScore(int Score);

	UFUNCTION()
	void SetContaminatorScore(int Score);

	UFUNCTION()
	void SetGameProgress(float GameProgress);

	UFUNCTION()
	void SetEffectivenessPercent(float EffectivenessPercent);

	UFUNCTION()
	void SetEnergyPercent(float EnergyPercent);

	UFUNCTION()
	void SetBubbleEffectivenessPercent(float BubbleEffectivenessPercent);

	UFUNCTION()
	void SetBubbleEffectivenessVisibility(bool bIsVisible);
};
