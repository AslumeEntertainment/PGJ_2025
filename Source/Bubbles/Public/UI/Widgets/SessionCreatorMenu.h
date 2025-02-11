// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Headers/GeneralDelegates.h"
#include "SessionCreatorMenu.generated.h"


class UEditableText;
class UCheckBox;
class UButton;

UCLASS()
class BUBBLES_API USessionCreatorMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Start;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Back;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UEditableText* ETXT_SessionName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* CB_IsPublic;

	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnBackClicked();

	virtual void NativeConstruct() override;

public:

	virtual void NativeOnInitialized() override;

	FSimpleSessionDataTransferSignature StartClicked;
	FVoidDataTransferSignature BackClicked;
};
