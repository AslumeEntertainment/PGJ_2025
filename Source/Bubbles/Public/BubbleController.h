// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Headers/GeneralDelegates.h"
#include "BubbleController.generated.h"

UENUM(BlueprintType)
enum class EInputMode : uint8
{
	GameOnly,
	UIOnly,
	GameAndUI,
};

UCLASS()
class BUBBLES_API ABubbleController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, Replicated, ReplicatedUsing = OnRep_CleannerPoints)
	int CleanerPoints;

	UPROPERTY(VisibleAnywhere, Replicated, ReplicatedUsing = OnRep_ContaminatorPoints)
	int ContaminatorPoints;

	UPROPERTY(VisibleAnywhere, Replicated, ReplicatedUsing = OnRep_GameProgress)
	float GameProgress;

	UPROPERTY(VisibleAnywhere, Replicated, ReplicatedUsing = OnRep_RemainingTime)
	int RemainingTime;

	UFUNCTION()
	void OnRep_CleannerPoints();

	UFUNCTION()
	void OnRep_ContaminatorPoints();

	UFUNCTION()
	void OnRep_GameProgress();

	UFUNCTION()
	void OnRep_RemainingTime();


public:

	UPROPERTY(Replicated)
	bool IsInputLocked = false;

	UPROPERTY(Replicated)
	int Team = 0;

	UFUNCTION(Client, Reliable)
	void Client_SetInputMode(EInputMode InputMode);

	UFUNCTION(Client, Reliable)
	void Client_SetupUIBindings();

	void OnPossess(APawn* InPawn) override;

	void AcknowledgePossession(APawn* P) override;

	UFUNCTION()
	void OnSessionMessegeReceived(FText Messege);

	UFUNCTION()
	void UpdateRemainingTime(int value);

	UFUNCTION(Client, Reliable)
	void HideStartingWidget();

	UFUNCTION(Client, Reliable)
	void ShowEndingWidget(int value);

	UFUNCTION()
	void OnCleanPoints(int points);

	UFUNCTION()
	void OnContaminPoints(int points);

	UFUNCTION()
	void OnProgress(float progress);

	UFUNCTION()
	void LeaveGame();

	UPROPERTY(BlueprintAssignable)
	FTextTransferSignature OnLobbyMessegeChanged;

	UPROPERTY(BlueprintAssignable)
	FIntegerTransferSignature OnCooldownUpdate;

	UPROPERTY(BlueprintAssignable)
	FVoidDataTransferSignature OnGameStarted;

	UPROPERTY(BlueprintAssignable)
	FTextTransferSignature OnGameEnd;

	UPROPERTY(BlueprintAssignable)
	FIntegerTransferSignature OnCleanerPointUpdate;

	UPROPERTY(BlueprintAssignable)
	FIntegerTransferSignature OnContaminatorPointUpdate;

	UPROPERTY(BlueprintAssignable)
	FFloatTransferSignature OnProgressUpdate;

};
