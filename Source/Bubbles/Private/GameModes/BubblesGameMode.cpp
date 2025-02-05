// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/BubblesGameMode.h"
#include "Characters/HumanBubble.h"
#include "BubbleController.h" 
#include "Kismet/GameplayStatics.h"
#include "Managers/PaintableItemSpawner.h"
#include "UObject/ConstructorHelpers.h"

ABubblesGameMode::ABubblesGameMode()
{

}

void ABubblesGameMode::BeginPlay()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ItemSpawnerClass, FoundActors);

	Spawner = Cast<APaintableItemSpawner>(FoundActors[0]);
	
	Spawner->OnCleanerPointsChanged;
	Spawner->OnContaminatorPointsChanged;
	Spawner->OnProgrssUpdated;

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

	OnLobbyMessegeChanged.AddDynamic(PC, &ABubbleController::OnSessionMessegeReceived);
	OnCooldownUpdate.AddDynamic(PC, &ABubbleController::UpdateRemainingTime);
	OnGameStart.AddDynamic(PC, &ABubbleController::HideStartingWidget);
	OnGameEnd.AddDynamic(PC, &ABubbleController::ShowEndingWidget);

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
	OnGameStart.Broadcast();

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

	TArray<ABubbleController*> PlayersArray;
	Players.GenerateKeyArray(PlayersArray);

	for (int i = 0; i < PlayersArray.Num(); i++)
	{
		if (IsValid(PlayersArray[i]) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("FORTNITE"));
			return;
		}
		AHumanBubble* Char;
		if (PlayersArray[i]->IsLocalController())
		{
			Char = World->SpawnActor<AHumanBubble>(CleanerClass, CleanerLocation, CleanerRotation, Params);
			Players.Emplace(PlayersArray[i], 1);
		}
		else
		{
			Char = World->SpawnActor<AHumanBubble>(ContaminatorClass, ContaminatorLocation, ContaminatorRotation, Params);
			Players.Emplace(PlayersArray[i], -1);
		}
		PlayersArray[i]->Possess(Char);
	}
	
}

void ABubblesGameMode::StartGame()
{
	GetWorldTimerManager().ClearTimer(PrepearingPeriodTimer);
	PrepearingPeriodTimer.Invalidate();

	OnLobbyMessegeChanged.Broadcast(FText::FromString("Game started"));
	OnCooldownUpdate.Broadcast(GamePeriodLenght);
	GetWorldTimerManager().SetTimer(GamePeriodTimer, this, &ABubblesGameMode::Countdown, 1, true, 1);
}

void ABubblesGameMode::Countdown() //tuka snqkuv delegat ima passvash ostavashtato vreme na vseki call
{
	GamePeriodLenght--;
	OnCooldownUpdate.Broadcast(GamePeriodLenght);
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
	OnGameEnd.Broadcast(Spawner->GetWinner());
}

void ABubblesGameMode::OnCleanPoints(int points)
{
	OnCleanerPointUpdate.Broadcast(points);
}

void ABubblesGameMode::OnContaminPoints(int points)
{
	OnContaminatorPointUpdate.Broadcast(points);
}

void ABubblesGameMode::OnProgress(float progress)
{
	OnProgressUpdate.Broadcast(progress);
}
