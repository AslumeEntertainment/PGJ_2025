// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UltimateAbilityWidget.generated.h"

class UTextBlock;

UCLASS()
class BUBBLES_API UUltimateAbilityWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TXT_TopText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TXT_BottomText;

public:

	void SetUltimateAbilityText(FText TopText, FText BottomText, FLinearColor TextColor);

};
