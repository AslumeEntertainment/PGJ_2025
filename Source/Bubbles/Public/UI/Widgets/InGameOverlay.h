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
	UProgressBar* GameProgressBar;

public:

	void SetTimerValue(int InTime);

	void SetCleanerScore(int Score);

	void SetContaminatorScore(int Score);

	void SetGameProgress(int GameProgress);
};
