// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Headers/GeneralDelegates.h"
#include "SessionSelectorMenu.generated.h"

class UButton;
class UScrollBox;
class USessionButtonWidget;

UCLASS()
class BUBBLES_API USessionSelectorMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Back;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* SessionScrollBox;

	UFUNCTION()
	void OnBackClicked();

public:

	virtual void NativeOnInitialized() override;

	void AddSessionButton(USessionButtonWidget* SessionButton);

	void ClearSessions();

	FVoidDataTransferSignature BackClicked;

};
