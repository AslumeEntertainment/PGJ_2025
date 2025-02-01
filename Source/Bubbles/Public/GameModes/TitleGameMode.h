// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "FindSessionsCallbackProxy.h"

#include "Headers/GeneralDelegates.h"

#include "TitleGameMode.generated.h"


UCLASS()
class BUBBLES_API ATitleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FBlueprintSessionResult> Sessions;

	UPROPERTY(BlueprintReadWrite)
	TArray<FText> SessionNames;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FTextArrayTransferSignature OnSessionsFound;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HostSession();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FindSessions();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void JoinSession(int SessionId);

	UFUNCTION()
	void ExitGame();

};
