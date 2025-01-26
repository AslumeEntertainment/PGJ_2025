// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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

	UFUNCTION(Client, Reliable)
	void Client_SetInputMode(EInputMode InputMode);

	void OnPossess(APawn* InPawn) override;

	UFUNCTION(/*Client, Reliable*/)
	void Client_OnSessionMessegeReceived(FText Messege);
};
