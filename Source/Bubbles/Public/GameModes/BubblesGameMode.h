// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Headers/GeneralDelegates.h"
#include "BubblesGameMode.generated.h"

class ABubbleController;
class AHumanBubble;

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
	float PrepearingPeriodLenght = 5;

	FTimerHandle GamePeriodTimer;
	float GamePeriodLenght = 300;

	void CheckLobbyReadiness();

	ABubblesGameMode();

	void PostLogin(APlayerController* NewPlayer) override;

	void PrepareGame();

	void StartGame();

	void Countdown();

	void EndGame();

	FTextTransferSignature OnLobbyMessegeChanged;
};



