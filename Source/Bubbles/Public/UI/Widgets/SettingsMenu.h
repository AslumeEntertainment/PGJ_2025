// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Headers/GeneralDelegates.h"
#include "SettingsMenu.generated.h"

class USlider;
class UButton;

UCLASS()
class BUBBLES_API USettingsMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Low;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Medium;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_High;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Epic;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Cinematic;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BTN_Back;

	UFUNCTION()
	void OnBackClicked();

	UFUNCTION()
	void OnLowClicked();

	UFUNCTION()
	void OnMediumClicked();

	UFUNCTION()
	void OnHighClicked();

	UFUNCTION()
	void OnEpicClicked();

	UFUNCTION()
	void OnCinematicClicked();

public:

	virtual void NativeOnInitialized() override;

	void SetCurrentGraphicsQualitySelected(int CurrentScalability);

	FIntegerTransferSignature GraphicsUpdated;
	FVoidDataTransferSignature BackClicked;

};
