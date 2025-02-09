// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/InGameHUD.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "UI/Widgets/InteractionWidget.h"
#include "UI/Widgets/GameOverWidget.h"
#include "UI/Widgets/LoadingScreen.h"
#include "UI/Widgets/InGameOverlay.h"
#include "Characters/HumanBubble.h"
#include "BubbleController.h"


void AInGameHUD::BeginPlay()
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		return;
	}

	if (PlayerOwner)
	{
		PlayerOwner->SetInputMode(FInputModeGameOnly());
		PlayerOwner->bShowMouseCursor = false;
	}

	InteractionWidget = CreateWidget<UInteractionWidget>(UGameplayStatics::GetGameInstance(World), InteractionWidgetClass);
	InGameOverlay = CreateWidget<UInGameOverlay>(UGameplayStatics::GetGameInstance(World), InGameOverlayClass);
	LoadingScreen = CreateWidget<ULoadingScreen>(UGameplayStatics::GetGameInstance(World), LoadingScreenClass);
	GameOverWidget = CreateWidget<UGameOverWidget>(UGameplayStatics::GetGameInstance(World), GameOverWidgetClass);
	
	ABubbleController* BubbleCont = Cast<ABubbleController>(PlayerOwner);
	if (IsValid(BubbleCont))
	{
		BubbleCont->OnCleanerPointUpdate.AddDynamic(InGameOverlay, &UInGameOverlay::SetCleanerScore);
		BubbleCont->OnContaminatorPointUpdate.AddDynamic(InGameOverlay, &UInGameOverlay::SetContaminatorScore);
		BubbleCont->OnProgressUpdate.AddDynamic(InGameOverlay, &UInGameOverlay::SetGameProgress);

		BubbleCont->OnGameStart.AddDynamic(this, &AInGameHUD::ShowInGameOverlay);
		BubbleCont->OnCooldownUpdate.AddDynamic(InGameOverlay, &UInGameOverlay::SetTimerValue);
		BubbleCont->OnGameEnd.AddDynamic(this, &AInGameHUD::ShowGameOverWidget);
	}

	ShowLoadingScreen(FText::FromString("Waiting for all players"));
}

void AInGameHUD::BindPlayerDelegatesToUI()
{
	if (IsValid(InGameOverlay) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AInGameHUD::BindPlayerDelegatesToUI IsValid(InGameOverlay) == false"));
		return;
	}

	AHumanBubble* HumanBubble = Cast<AHumanBubble>(PlayerOwner->GetPawn());
	if (IsValid(HumanBubble) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AInGameHUD::BindPlayerDelegatesToUI IsValid(HumanBubble) == false"));
		return;
	}
	if (HumanBubble->OnEffectivenessUpdated.IsBound() == false)
	{
		HumanBubble->OnEffectivenessUpdated.AddDynamic(InGameOverlay, &UInGameOverlay::SetEffectivenessPercent);
	}
	if (HumanBubble->OnEnergyUpdated.IsBound() == false)
	{
		HumanBubble->OnEnergyUpdated.AddDynamic(InGameOverlay, &UInGameOverlay::SetEnergyPercent);
	}
	if (HumanBubble->InteractIndicationTextDelegate.IsBound() == false)
	{
		HumanBubble->InteractIndicationTextDelegate.AddDynamic(InteractionWidget, &UInteractionWidget::SetInteractionText);
	}
	HumanBubble->BroadcastInitialValues();
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

	FTimerHandle BindDelegatesTimer;
	GetWorldTimerManager().SetTimer(BindDelegatesTimer, this, &AInGameHUD::BindPlayerDelegatesToUI, 0.01, false);
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
