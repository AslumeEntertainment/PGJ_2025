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

	UPROPERTY(EditAnywhere)
	int PlayersNeededForSession = 2;

	UPROPERTY(VisibleAnywhere)
	int ContaminateorTeam = -1;

	UPROPERTY(VisibleAnywhere)
	TMap<ABubbleController*, int> Players;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APaintableItemSpawner> ItemSpawnerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AHumanBubble> CleanerClass;

	UPROPERTY(EditAnywhere)
	FVector CleanerLocation;

	UPROPERTY(EditAnywhere)
	FRotator CleanerRotation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AHumanBubble> ContaminatorClass;

	UPROPERTY(EditAnywhere)
	FVector ContaminatorLocation;

	UPROPERTY(EditAnywhere)
	FRotator ContaminatorRotation;

	FTimerHandle PrepearingPeriodTimer;
	UPROPERTY(EditAnywhere)
	float PrepearingPeriodLenght = 5;

	FTimerHandle GamePeriodTimer;
	UPROPERTY(EditAnywhere)
	float GamePeriodLenght = 300;

	UPROPERTY()
	APaintableItemSpawner* Spawner;

	void CheckLobbyReadiness();

	ABubblesGameMode();

	void BeginPlay() override;

	void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION()
	void PrepareGame();

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void Countdown();

	UFUNCTION()
	void EndGame();

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



