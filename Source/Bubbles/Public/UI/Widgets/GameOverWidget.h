// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Headers/GeneralDelegates.h"
#include "GameOverWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class BUBBLES_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TXT_GameOverMessage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_PlayAgain;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Quit;

	UFUNCTION()
	void OnPlayAgainClicked();

	UFUNCTION()
	void OnQuitClicked();

public:

	virtual void NativeOnInitialized() override;

	void SetGameOverMessage(FText GameOverMessage);

	FVoidDataTransferSignature PlayAgainClicked;
	FVoidDataTransferSignature QuitClicked;

};
