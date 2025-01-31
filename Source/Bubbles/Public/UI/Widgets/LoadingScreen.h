// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreen.generated.h"

class UTextBlock;

UCLASS()
class BUBBLES_API ULoadingScreen : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TXT_LoadingText;

public:

	void SetLoadingText(FText LoadingText);

};
