// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TitleGameMode.h"

#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"


void ATitleGameMode::BeginPlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (IsValid(PlayerController) == false)
	{
		return;
	}

	TArray<AActor*> CamerasInLevel;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), CamerasInLevel);

	for (AActor* Actor : CamerasInLevel)
	{
		ACameraActor* CameraActor = Cast<ACameraActor>(Actor);
		if (IsValid(CameraActor))
		{
			PlayerController->SetViewTargetWithBlend(CameraActor, 0, EViewTargetBlendFunction::VTBlend_Linear);
			return;
		}
	}
}

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

void ATitleGameMode::SetScalability(int NewValue)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
	if (IsValid(UserSettings) == false) 
	{
		UE_LOG(LogTemp, Error, TEXT("ATitleGameMode::ChangeScalability IsValid(UserSettings) == false"));
		return;
	}

	if (NewValue != UserSettings->GetOverallScalabilityLevel())
	{
		UserSettings->SetOverallScalabilityLevel(NewValue);
		UserSettings->ApplyNonResolutionSettings();
		UserSettings->SaveSettings();
	}
}

int ATitleGameMode::GetScalability()
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();

	return IsValid(UserSettings) ? UserSettings->GetOverallScalabilityLevel() : -1;
}

void ATitleGameMode::ExitGame()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) 
	{
		PlayerController->ConsoleCommand("quit");
	}
}
