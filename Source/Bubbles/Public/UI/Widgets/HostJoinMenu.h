// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Headers/GeneralDelegates.h"
#include "HostJoinMenu.generated.h"

class UButton;

UCLASS()
class BUBBLES_API UHostJoinMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Host;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Join;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Back;

	UFUNCTION()
	void OnHostClicked();

	UFUNCTION()
	void OnJoinClicked();

	UFUNCTION()
	void OnBackClicked();

public:

	virtual void NativeOnInitialized() override;

	FVoidDataTransferSignature HostClicked;
	FVoidDataTransferSignature JoinClicked;
	FVoidDataTransferSignature BackClicked;
};
