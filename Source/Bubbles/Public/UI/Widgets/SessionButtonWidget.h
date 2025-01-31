// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Headers/GeneralDelegates.h"
#include "SessionButtonWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class BUBBLES_API USessionButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Session;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TXT_SessionName;

	UFUNCTION()
	void OnSessionClicked();

public:

	virtual void NativeOnInitialized() override;

	void SetSessionName(FText SessionName);

	FIntegerTransferSignature SessionClicked;
};
