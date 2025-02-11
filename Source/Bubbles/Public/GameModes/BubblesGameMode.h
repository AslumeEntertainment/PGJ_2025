// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Headers/GeneralDelegates.h"
#include "BubblesGameMode.generated.h"

class ABubbleController;
class AHumanBubble;
class APaintableItemSpawner;

UCLASS(minimalapi)
class ABubblesGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	TMap<ABubbleController*, int> Players;

	UPROPERTY()
	APaintableItemSpawner* Spawner;

	UPROPERTY(EditAnywhere, Category = "GameDefaults")
	int PlayersNeededForSession = 2;

	FTimerHandle PrepearingPeriodTimer;
	UPROPERTY(EditAnywhere, Category = "GameDefaults")
	float PrepearingPeriodLenght = 5;

	FTimerHandle GamePeriodTimer;
	UPROPERTY(EditAnywhere, Category = "GameDefaults")
	float GamePeriodLenght = 300;

	UPROPERTY(EditAnywhere, Category = "GameDefaults")
	TSubclassOf<APaintableItemSpawner> ItemSpawnerClass;

	UPROPERTY(VisibleAnywhere, Category = "GameDefaults|Cleaner")
	int CleanerTeam = 1;

	UPROPERTY(EditAnywhere, Category = "GameDefaults|Cleaner")
	TSubclassOf<AHumanBubble> CleanerClass;

	UPROPERTY(EditAnywhere, Category = "GameDefaults|Cleaner")
	FVector CleanerLocation;

	UPROPERTY(EditAnywhere, Category = "GameDefaults|Cleaner")
	FRotator CleanerRotation;

	UPROPERTY(VisibleAnywhere, Category = "GameDefaults|Contaminator")
	int ContaminateorTeam = -1;

	UPROPERTY(EditAnywhere, Category = "GameDefaults|Contaminator")
	TSubclassOf<AHumanBubble> ContaminatorClass;

	UPROPERTY(EditAnywhere, Category = "GameDefaults|Contaminator")
	FVector ContaminatorLocation;

	UPROPERTY(EditAnywhere, Category = "GameDefaults|Contaminator")
	FRotator ContaminatorRotation;

	void CheckLobbyReadiness();

	ABubblesGameMode();

	void BeginPlay() override;

	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;

	UFUNCTION()
	void PrepareGame();

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void Countdown();

	UFUNCTION()
	void EndGame();

	UFUNCTION()
	void ResetGame();

	UFUNCTION()
	void OnCleanPoints(int points);

	UFUNCTION()
	void OnContaminPoints(int points);

	UFUNCTION()
	void OnProgress(float progress);
	
	FTextTransferSignature OnLobbyMessegeChanged;
	
	FIntegerTransferSignature OnCooldownUpdate;

	FIntegerTransferSignature OnCleanerPointUpdate;

	FIntegerTransferSignature OnContaminatorPointUpdate;

	FFloatTransferSignature OnProgressUpdate;
	
	FVoidDataTransferSignature OnGameStart;
	
	FIntegerTransferSignature OnGameEnd;
};



