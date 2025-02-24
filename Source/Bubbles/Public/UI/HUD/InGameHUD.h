// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

class UInGameOverlay;
class ULoadingScreen;
class UInteractionWidget;
class UGameOverWidget;

UCLASS()
class BUBBLES_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClasses");
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClasses");
	TSubclassOf<UInGameOverlay> InGameOverlayClass;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClasses");
	TSubclassOf<ULoadingScreen> LoadingScreenClass;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClasses");
	TSubclassOf<UGameOverWidget> GameOverWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInteractionWidget* InteractionWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInGameOverlay* InGameOverlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ULoadingScreen* LoadingScreen;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UGameOverWidget* GameOverWidget;

public:

	UFUNCTION()
	void BindPlayerDelegatesToUI();

	UFUNCTION()
	void ShowInGameOverlay();

	UFUNCTION()
	void ShowLoadingScreen(FText LoadingText = FText::FromString("Loading..."));

	UFUNCTION()
	void RemoveLoadingScreen();

	UFUNCTION()
	void ShowInteractionWidget();

	UFUNCTION()
	void HideInteractionWidget();

	UFUNCTION()
	void ShowGameOverWidget(FText GameOverMessage);

	UFUNCTION()
	void ClearScreen();

};
