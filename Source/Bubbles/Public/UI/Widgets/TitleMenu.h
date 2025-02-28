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
	UButton* BTN_Host;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Join;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Settings;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Exit;

	UFUNCTION()
	void OnHostClicked();

	UFUNCTION()
	void OnJoinClicked();

	UFUNCTION()
	void OnSettingsClicked();

	UFUNCTION()
	void OnExitClicked();

public:

	virtual void NativeOnInitialized() override;

	FVoidDataTransferSignature HostClicked;
	FVoidDataTransferSignature JoinClicked;
	FVoidDataTransferSignature SettingsClicked;
	FVoidDataTransferSignature ExitClicked;
};
