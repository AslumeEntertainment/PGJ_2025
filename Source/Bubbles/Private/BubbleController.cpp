// Fill out your copyright notice in the Description page of Project Settings.


#include "BubbleController.h"

#include "Headers/GeneralDelegates.h"
#include "Net/UnrealNetwork.h"

#include "Characters/HumanBubble.h"
#include "UI/HUD/InGameHUD.h"	

void ABubbleController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ABubbleController, IsInputLocked, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ABubbleController, Team, COND_None, REPNOTIFY_Always);
}
void ABubbleController::Client_SetInputMode_Implementation(EInputMode  InputMode)
{
	switch (InputMode)
	{
		case EInputMode::GameOnly: SetInputMode(FInputModeGameOnly()); IsInputLocked = false;  break;
		case EInputMode::GameAndUI: SetInputMode(FInputModeGameAndUI()); IsInputLocked = false; break;
		case EInputMode::UIOnly: SetInputMode(FInputModeUIOnly()); IsInputLocked = true; break;
	}
}
void ABubbleController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Client_SetInputMode(EInputMode::GameOnly);
}

void ABubbleController::OnSessionMessegeReceived(FText Messege)//_Implementation(FText Messege)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Client Recieved: %s"), *GetName(), *Messege.ToString());
	//OnLobbyMessegeChanged.Broadcast(Messege);
}

void ABubbleController::UpdateRemainingTime_Implementation(int value)
{
	//UE_LOG(LogTemp, Error, TEXT("%s: Remaining Time %d"), *GetName(), value);
	OnCooldownUpdate.Broadcast(value);
}

void ABubbleController::HideStartingWidget_Implementation()
{
	if (OnGameStart.IsBound())
	{
		OnGameStart.Broadcast();
	}
	else UE_LOG(LogTemp, Error, TEXT("%s: ABubbleController::HideStartingWidget OnGameStart is not bound!!!"), *GetName());
	
	UE_LOG(LogTemp, Warning, TEXT("%s: ABubbleController::HideStartingWidget"), *GetName());
}

void ABubbleController::ShowEndingWidget_Implementation(int value)
{
	if (value == 0)
	{
		OnGameEnd.Broadcast(FText::FromString("You Tied"));
	}
	else
	{
		OnGameEnd.Broadcast(FText::FromString("You " + FString(FMath::Sign(Team) == FMath::Sign(value) ? "Win" : "Lose")));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%s: ABubbleController::ShowEndingWidget"), *GetName());
}

void ABubbleController::OnCleanPoints(int points)
{
	OnCleanerPointUpdate.Broadcast(points);
}

void ABubbleController::OnContaminPoints(int points)
{
	OnContaminatorPointUpdate.Broadcast(points);
}

void ABubbleController::OnProgress(float progress)
{
	OnProgressUpdate.Broadcast(progress);
}
