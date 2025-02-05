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
	
public:

	UPROPERTY(Replicated)
	bool IsInputLocked = false;

	UPROPERTY(Replicated)
	int Team = 0;

	UFUNCTION(Client, Reliable)
	void Client_SetInputMode(EInputMode InputMode);

	void OnPossess(APawn* InPawn) override;

	UFUNCTION(/*Client, Reliable*/)
	void OnSessionMessegeReceived(FText Messege);

	UFUNCTION(Client, Reliable)
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

	UPROPERTY(BlueprintAssignable)
	FTextTransferSignature OnLobbyMessegeChanged;

	UPROPERTY(BlueprintAssignable)
	FIntegerTransferSignature OnCooldownUpdate;

	UPROPERTY(BlueprintAssignable)
	FVoidDataTransferSignature OnGameStart;

	UPROPERTY(BlueprintAssignable)
	FTextTransferSignature OnGameEnd;

	UPROPERTY(BlueprintAssignable)
	FIntegerTransferSignature OnCleanerPointUpdate;

	UPROPERTY(BlueprintAssignable)
	FIntegerTransferSignature OnContaminatorPointUpdate;

	UPROPERTY(BlueprintAssignable)
	FFloatTransferSignature OnProgressUpdate;

};
