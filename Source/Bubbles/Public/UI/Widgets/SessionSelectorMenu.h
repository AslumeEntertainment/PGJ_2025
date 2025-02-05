// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Headers/GeneralDelegates.h"
#include "SessionSelectorMenu.generated.h"

class UButton;
class UScrollBox;
class UWidgetSwitcher;
class USessionButtonWidget;

UCLASS()
class BUBBLES_API USessionSelectorMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClasses");
	TSubclassOf<USessionButtonWidget> SessionButtonWidgetClass;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Back;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* SessionScrollBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* SessionWidgetSwitcher;

	UFUNCTION()
	void OnBackClicked();

	virtual void NativeConstruct() override;

public:

	virtual void NativeOnInitialized() override;

	USessionButtonWidget* AddSessionButton(FText SessionName);

	void ClearSessions();

	FVoidDataTransferSignature BackClicked;

};
