// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TitleGameMode.h"

#include "Kismet/GameplayStatics.h"


void ATitleGameMode::HostSession_Implementation(const FString& SessionName, bool bIsPublic)
{
	UE_LOG(LogTemp, Error, TEXT("No Blueprint implementation exists for ATitleGameMode::HostSession"));
}

void ATitleGameMode::JoinSession_Implementation(int SessionId)
{
	UE_LOG(LogTemp, Error, TEXT("No Blueprint implementation exists for ATitleGameMode::JoinSession"));
}

void ATitleGameMode::FindSessions_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("No Blueprint implementation exists for ATitleGameMode::FindSessions"));
}

void ATitleGameMode::ExitGame()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) 
	{
		PlayerController->ConsoleCommand("quit");
	}
}
