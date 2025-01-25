// Fill out your copyright notice in the Description page of Project Settings.


#include "BubbleController.h"


#include "Net/UnrealNetwork.h"

void ABubbleController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ABubbleController, IsInputLocked, COND_None, REPNOTIFY_Always);
}
void ABubbleController::Client_SetInputMode_Implementation(EInputMode  InputMode)
{
	switch (InputMode)
	{
		case EInputMode::GameOnly: SetInputMode(FInputModeGameOnly()); break;
		case EInputMode::GameAndUI: SetInputMode(FInputModeGameAndUI()); break;
		case EInputMode::UIOnly: SetInputMode(FInputModeUIOnly()); break;
	}
}
