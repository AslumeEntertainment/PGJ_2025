// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/BubblesGameMode.h"
#include "Characters/HumanBubble.h"
#include "BubbleController.h" 
#include "UObject/ConstructorHelpers.h"

ABubblesGameMode::ABubblesGameMode()
{

}


void ABubblesGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("LOGGIN IN %s"), *NewPlayer->GetName());


	ABubbleController* PC = Cast<ABubbleController>(NewPlayer);
	if (IsValid(PC) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AGameplayGameMode::PostLogin IsValid(PC) == false  PC is not of class ASG_PlayerController"));
		return;
	}

	if (true)
	{
		UE_LOG(LogTemp, Warning, TEXT("ADDING %s TO PLAYER LIST"), *PC->GetName());
	}

	//AllPlayers.Add(PC->GetPlayerState<APlayerState>()->GetPlayerName(), PC);
	//SendPlayerListUpdate();

	Players.Add(PC, 0);

	OnLobbyMessegeChanged.AddDynamic(PC, &ABubbleController::Client_OnSessionMessegeReceived);

	PC->Client_SetInputMode(EInputMode::UIOnly);//Must be GameOnly by default
	//PC->OnConnectionCleanUp.AddDynamic(this, &AGameplayGameMode::UnRegisterRequest);

	CheckLobbyReadiness();
}


void ABubblesGameMode::CheckLobbyReadiness()
{
	FText Messege = FText::FromString(FString("Wating for players ") + FString::FromInt(Players.Num()) + FString("/") + FString::FromInt(PlayersNeededForSession));
	
	OnLobbyMessegeChanged.Broadcast(Messege);

	if (Players.Num() < PlayersNeededForSession)
	{
		return;
	}

	OnLobbyMessegeChanged.Broadcast(FText::FromString("Preparing game..."));

	GetWorldTimerManager().SetTimer(PrepearingPeriodTimer, this, &ABubblesGameMode::StartGame, PrepearingPeriodLenght, false);

	PrepareGame();
}

void ABubblesGameMode::PrepareGame()
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AGameplayGameMode::PostLogin IsValid(World) == false"));
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (TPair<ABubbleController*, int> Player : Players)
	{
		AHumanBubble* Char;
		if (Player.Key->IsLocalController())
		{
			Char = World->SpawnActor<AHumanBubble>(CleanerClass, CleanerLocation, CleanerRotation, Params);
			Players.Emplace(Player.Key, 1);
		}
		else
		{
			Char = World->SpawnActor<AHumanBubble>(ContaminatorClass, ContaminatorLocation, ContaminatorRotation, Params);
			Players.Emplace(Player.Key, -1);
		}
		Player.Key->Possess(Char);
	}
	
}

void ABubblesGameMode::StartGame()
{
	GetWorldTimerManager().ClearTimer(PrepearingPeriodTimer);
	PrepearingPeriodTimer.Invalidate();

	OnLobbyMessegeChanged.Broadcast(FText::FromString("Game started"));
	GetWorldTimerManager().SetTimer(GamePeriodTimer, this, &ABubblesGameMode::Countdown, 1, true, 1);
}

void ABubblesGameMode::Countdown() //tuka snqkuv delegat ima passvash ostavashtato vreme na vseki call
{
	GamePeriodLenght--;
	if (GamePeriodLenght > 0)
	{
		return;
	}
	GetWorldTimerManager().ClearTimer(GamePeriodTimer);
	GamePeriodTimer.Invalidate();
	EndGame();
}

void ABubblesGameMode::EndGame() //tuka trqbva da podadesh na playerite widgeti dase vurnat kum main menu
{
	OnLobbyMessegeChanged.Broadcast(FText::FromString("Game ended"));
}
