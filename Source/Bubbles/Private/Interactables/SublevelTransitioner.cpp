// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/SublevelTransitioner.h"

#include "GameFramework/PlayerController.h"
#include "Camera/CameraActor.h"

#include "Characters/FlatBubbleCharacter.h"
#include "UI/HUD/InGameHUD.h"

void ASublevelTransitioner::InteractRequest(AController* InteractingCharacter)
{
	//play anim

	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ASublevelTransitioner::InteractRequest IsValid(World) == false"));
		return;
	}

	APlayerController* PlayerCont = Cast<APlayerController>(InteractingCharacter);
	if (IsValid(PlayerCont) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ASublevelTransitioner::InteractRequest IsValid(PlayerCont) == false"));
		return;
	}

	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AFlatBubbleCharacter* FlatBubble = World->SpawnActor<AFlatBubbleCharacter>(FlatBubbleClass, SublevelStartingLocation, FRotator(0, 0, 0), SpawnParams);
	if (IsValid(FlatBubble) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ASublevelTransitioner::InteractRequest IsValid(FlatBubble) == false"));
		return;
	}

	PlayerCont->Possess(FlatBubble);
	PlayerCont->SetViewTargetWithBlend(SublevelCamera);


}

bool ASublevelTransitioner::bCanInteract(AController* InteractingCharacter)
{
	return true;
}
