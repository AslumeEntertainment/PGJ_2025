// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/InGameHUD.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "UI/Widgets/InteractionWidget.h"
#include "UI/Widgets/GameOverWidget.h"
#include "UI/Widgets/LoadingScreen.h"
#include "UI/Widgets/InGameOverlay.h"
#include "Characters/FlatBubbleCharacter.h"
#include "Characters/HumanBubble.h"
#include "BubbleController.h"


void AInGameHUD::BeginPlay()
{
	if (PlayerOwner)
	{
		PlayerOwner->SetInputMode(FInputModeGameOnly());
		PlayerOwner->bShowMouseCursor = false;
	}
}

void AInGameHUD::BindControllerDelegatesToUI(ABubbleController* Controller)
{
	if (IsValid(Controller) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AInGameHUD::BindControllerDelegatesToUI IsValid(Controller) == false"));
		return;
	}

	InteractionWidget = CreateWidget<UInteractionWidget>(Controller, InteractionWidgetClass);
	InGameOverlay = CreateWidget<UInGameOverlay>(Controller, InGameOverlayClass);
	LoadingScreen = CreateWidget<ULoadingScreen>(Controller, LoadingScreenClass);
	GameOverWidget = CreateWidget<UGameOverWidget>(Controller, GameOverWidgetClass);

	ShowLoadingScreen(FText::FromString("Waiting for all players"));

	if (IsValid(InGameOverlay) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AInGameHUD::BindControllerDelegatesToUI IsValid(InGameOverlay) == false"));
		return;
	}

	Controller->OnCleanerPointUpdate.AddDynamic(InGameOverlay, &UInGameOverlay::SetCleanerScore);
	Controller->OnContaminatorPointUpdate.AddDynamic(InGameOverlay, &UInGameOverlay::SetContaminatorScore);
	Controller->OnProgressUpdate.AddDynamic(InGameOverlay, &UInGameOverlay::SetGameProgress);

	Controller->OnGameStarted.AddDynamic(this, &AInGameHUD::ShowInGameOverlay);
	Controller->OnCooldownUpdate.AddDynamic(InGameOverlay, &UInGameOverlay::SetTimerValue);
	Controller->OnGameEnd.AddDynamic(this, &AInGameHUD::ShowGameOverWidget);

	GameOverWidget->QuitClicked.AddDynamic(Controller, &ABubbleController::LeaveGame);
}

void AInGameHUD::BindPawnDelegatesToUI(AHumanBubble* Pawn)
{
	if (IsValid(Pawn) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AInGameHUD::BindPlayerDelegatesToUI IsValid(HumanBubble) == false"));
		return;
	}
	if (Pawn->OnEffectivenessUpdated.IsBound() == false)
	{
		Pawn->OnEffectivenessUpdated.AddDynamic(InGameOverlay, &UInGameOverlay::SetEffectivenessPercent);
	}
	if (Pawn->OnEnergyUpdated.IsBound() == false)
	{
		Pawn->OnEnergyUpdated.AddDynamic(InGameOverlay, &UInGameOverlay::SetEnergyPercent);
	}
	if (Pawn->InteractIndicationTextDelegate.IsBound() == false)
	{
		Pawn->InteractIndicationTextDelegate.AddDynamic(InteractionWidget, &UInteractionWidget::SetInteractionText);
	}

	InGameOverlay->SetBubbleEffectivenessVisibility(false);
	Pawn->BroadcastInitialValues();
}

void AInGameHUD::Bind2DPawnDelegatesToUI(AFlatBubbleCharacter* Pawn)
{
	if (IsValid(Pawn) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AInGameHUD::BindPlayerDelegatesToUI IsValid(HumanBubble) == false"));
		return;
	}
	if (Pawn->OnEffectivenessUpdated.IsBound() == false)
	{
		Pawn->OnEffectivenessUpdated.AddDynamic(InGameOverlay, &UInGameOverlay::SetBubbleEffectivenessPercent);
	}
	
	InGameOverlay->SetBubbleEffectivenessVisibility(true);
	Pawn->BroadcastInitialValues();
}

void AInGameHUD::ShowInGameOverlay()
{
	ClearScreen();

	if (IsValid(InGameOverlay) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AInGameHUD::ShowInGameOverlay IsValid(InGameOverlay) == false"));
		return;
	}

	InGameOverlay->AddToViewport();
	ShowInteractionWidget();
}

void AInGameHUD::ShowLoadingScreen(FText LoadingText)
{
	if (IsValid(LoadingScreen) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AInGameHUD::ShowLoadingScreen IsValid(LoadingScreen) == false"));
		return;
	}

	LoadingScreen->SetLoadingText(LoadingText);
	if (LoadingScreen->IsInViewport() == false) LoadingScreen->AddToViewport(10);
}

void AInGameHUD::RemoveLoadingScreen()
{
	if (IsValid(LoadingScreen) && LoadingScreen->IsInViewport())
	{
		LoadingScreen->RemoveFromParent();
	}
}

void AInGameHUD::ShowInteractionWidget()
{
	UE_LOG(LogTemp, Error, TEXT("AInGameHUD::ShowInteractionWidget"));

	if (IsValid(InteractionWidget) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AInGameHUD::ShowInteractionWidget IsValid(InteractionWidget) == false"));
		return;
	}
	if (InteractionWidget->IsInViewport())
	{
		return;
	}
	
	InteractionWidget->AddToViewport();
}

void AInGameHUD::HideInteractionWidget()
{
	if (IsValid(InteractionWidget) == false)
	{
		return;
	}

	InteractionWidget->RemoveFromParent();
}

void AInGameHUD::ShowGameOverWidget(FText GameOverMessage)
{
	ClearScreen();

	if (IsValid(GameOverWidget) == false || IsValid(PlayerOwner) == false)
	{
		return;
	}

	PlayerOwner->SetInputMode(FInputModeUIOnly());
	PlayerOwner->SetShowMouseCursor(true);

	GameOverWidget->SetGameOverMessage(GameOverMessage);
	GameOverWidget->AddToViewport();
}

void AInGameHUD::ClearScreen()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
}
