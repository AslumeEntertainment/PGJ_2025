// Fill out your copyright notice in the Description page of Project Settings.


#include "BubbleController.h"

#include "Headers/GeneralDelegates.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "Characters/HumanBubble.h"
#include "Characters/FlatBubbleCharacter.h"
#include "UI/HUD/InGameHUD.h"

void ABubbleController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ABubbleController, CleanerPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ABubbleController, ContaminatorPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ABubbleController, GameProgress, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ABubbleController, RemainingTime, COND_None, REPNOTIFY_Always);

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

void ABubbleController::Client_SetupUIBindings_Implementation()
{
	AInGameHUD* InGameHUD = Cast<AInGameHUD>(GetHUD());
	if (IsValid(InGameHUD) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ABubbleController::Client_SetupUIBindings_Implementation IsValid(InGameHUD) == false"))
		return;
	}
	InGameHUD->BindControllerDelegatesToUI(this);
}

void ABubbleController::OnRep_CleannerPoints()
{
	OnCleanerPointUpdate.Broadcast(CleanerPoints);
}

void ABubbleController::OnRep_ContaminatorPoints()
{
	OnContaminatorPointUpdate.Broadcast(ContaminatorPoints);
}

void ABubbleController::OnRep_GameProgress()
{
	OnProgressUpdate.Broadcast(GameProgress);
}

void ABubbleController::OnRep_RemainingTime()
{
	OnCooldownUpdate.Broadcast(RemainingTime);
}

void ABubbleController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Client_SetInputMode(EInputMode::GameOnly);
}

void ABubbleController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	AInGameHUD* InGameHUD = GetHUD<AInGameHUD>();
	if (IsValid(InGameHUD) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ABubbleController::AcknowledgePossession IsValid(InGameHUD) == false"));
		return;
	}

	ABubbleCharacter* BubblePawn = Cast<ABubbleCharacter>(P);
	if (IsValid(BubblePawn) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ABubbleController::AcknowledgePossession IsValid(BubblePawn) == false"));
		return;
	}
	
	BindPawnMappingContext(BubblePawn);

	if (IsValid(Cast<AHumanBubble>(BubblePawn)))
	{
		InGameHUD->BindPawnDelegatesToUI(Cast<AHumanBubble>(BubblePawn));
		InGameHUD->ShowInteractionWidget();
	}
	else if (IsValid(Cast<AFlatBubbleCharacter>(BubblePawn)))
	{
		InGameHUD->Bind2DPawnDelegatesToUI(Cast<AFlatBubbleCharacter>(BubblePawn));
		InGameHUD->HideInteractionWidget();
	}
}

void ABubbleController::BindPawnMappingContext(ABubbleCharacter* BubblePawn)
{
	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(InputSystem) == false)
	{
		Client_BindPawnMappingContext(BubblePawn);
		return;
	}
	InputSystem->AddMappingContext(BubblePawn->GetDefaultInputMappingContext(), 0);
}

void ABubbleController::Client_BindPawnMappingContext_Implementation(ABubbleCharacter* BubblePawn)
{
	BindPawnMappingContext(BubblePawn);
}

void ABubbleController::UnbindPawnMappingContext(ABubbleCharacter* BubblePawn)
{
	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(InputSystem) == false)
	{
		Client_UnbindPawnMappingContext(BubblePawn);
		return;
	}

	InputSystem->RemoveMappingContext(BubblePawn->GetDefaultInputMappingContext());
}

void ABubbleController::Client_UnbindPawnMappingContext_Implementation(ABubbleCharacter* BubblePawn)
{
	UnbindPawnMappingContext(BubblePawn);
}

void ABubbleController::OnSessionMessegeReceived(FText Messege)//_Implementation(FText Messege)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Client Recieved: %s"), *GetName(), *Messege.ToString());
	//OnLobbyMessegeChanged.Broadcast(Messege);
}

void ABubbleController::UpdateRemainingTime(int value)
{
	//UE_LOG(LogTemp, Error, TEXT("%s: Remaining Time %d"), *GetName(), value);
	RemainingTime = value;
	OnCooldownUpdate.Broadcast(RemainingTime);
}

void ABubbleController::HideStartingWidget_Implementation()
{
	OnGameStarted.Broadcast();
	//UE_LOG(LogTemp, Warning, TEXT("%s: ABubbleController::HideStartingWidget"), *GetName());
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
	CleanerPoints = points;
	OnCleanerPointUpdate.Broadcast(CleanerPoints);
}

void ABubbleController::OnContaminPoints(int points)
{
	ContaminatorPoints = points;
	OnContaminatorPointUpdate.Broadcast(ContaminatorPoints);
}

void ABubbleController::OnProgress(float progress)
{
	GameProgress = progress;
	OnProgressUpdate.Broadcast(GameProgress);
}

void ABubbleController::LeaveGame()
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ABubbleController::LeaveGame IsValid(World) == false"));
		return;
	}

	UGameplayStatics::OpenLevel(World, "TitleLevel");
}
