// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/InGameHUD.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "UI/Widgets/InteractionWidget.h"
#include "UI/Widgets/LoadingScreen.h"
#include "UI/Widgets/InGameOverlay.h"
#include "Characters/BubbleCharacter.h"
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
	
	ABubbleController* BubbleCont = Cast<ABubbleController>(PlayerOwner);
	if (IsValid(BubbleCont))
	{
		BubbleCont->OnGameStart.AddDynamic(this, &AInGameHUD::ShowInGameOverlay);
		BubbleCont->OnCleanerPointUpdate.AddDynamic(InGameOverlay, &UInGameOverlay::SetCleanerScore);
		BubbleCont->OnContaminatorPointUpdate.AddDynamic(InGameOverlay, &UInGameOverlay::SetContaminatorScore);
		BubbleCont->OnProgressUpdate.AddDynamic(InGameOverlay, &UInGameOverlay::SetGameProgress);
		BubbleCont->OnCooldownUpdate.AddDynamic(InGameOverlay, &UInGameOverlay::SetTimerValue);
	}

	ShowLoadingScreen(FText::FromString("Waiting for all players"));
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
	ToggleInteractionWidget(true);
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

void AInGameHUD::ToggleInteractionWidget(bool bShouldShow)
{
	if (IsValid(InteractionWidget) == false || IsValid(PlayerOwner) == false)
	{
		return;
	}

	if ((bShouldShow && InteractionWidget->IsInViewport()) || !bShouldShow && !InteractionWidget->IsInViewport())
	{
		return;
	}
	
	if (bShouldShow)
	{
		ABubbleCharacter* BubbleCharacter = Cast<ABubbleCharacter>(PlayerOwner->GetPawn());
		if (IsValid(BubbleCharacter) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AInGameHUD::ToggleInteractionWidget IsValid(BubbleCharacter) == false"))
			return;
		}
		if (BubbleCharacter->InteractIndicationTextDelegate.IsBound() == false)
		{
			BubbleCharacter->InteractIndicationTextDelegate.AddDynamic(InteractionWidget, &UInteractionWidget::SetInteractionText);
		}
		InteractionWidget->AddToViewport();
	}
	else
	{
		InteractionWidget->RemoveFromParent();
	}
}

void AInGameHUD::ClearScreen()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
}
