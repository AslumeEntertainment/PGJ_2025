// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class UTextBlock;

UCLASS()
class BUBBLES_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* InteractionText;

public:

	UFUNCTION()
	void SetInteractionText(FText Text);


};
