// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Headers/GeneralDelegates.h"
#include "TitleMenu.generated.h"

class UButton;

UCLASS()
class BUBBLES_API UTitleMenu : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Start;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Exit;

	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnExitClicked();

public:

	virtual void NativeOnInitialized() override;

	FVoidDataTransferSignature StartClicked;
	FVoidDataTransferSignature ExitClicked;
};
