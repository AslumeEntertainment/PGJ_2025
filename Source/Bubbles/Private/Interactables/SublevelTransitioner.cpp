// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/SublevelTransitioner.h"

#include "GameFramework/PlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAS/BubbleAttributeSet.h"
#include "Camera/CameraActor.h"

#include "Characters/FlatBubbleCharacter.h"
#include "Characters/HumanBubble.h"
#include "BubbleController.h"
#include "UI/HUD/InGameHUD.h"


void ASublevelTransitioner::ContinueInteraction(ABubbleController* PlayerCont, AHumanBubble* PlayerPawn)
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ASublevelTransitioner::InteractRequest IsValid(World) == false"));
		return;
	}

	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AFlatBubbleCharacter* FlatBubble = World->SpawnActor<AFlatBubbleCharacter>(PlayerPawn->FlatBubbleClass, SublevelStartingLocation, FlatBubbleSpawnRotation, SpawnParams);
	if (IsValid(FlatBubble) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ASublevelTransitioner::InteractRequest IsValid(FlatBubble) == false"));
		return;
	}
	
	PlayerCont->SetIsInputEnabled(false);
	PlayerCont->Possess(FlatBubble);

	PlayerCont->SetViewTargetWithBlend(SublevelCamera);
	PlayerCont->ClientSetRotation(FlatBubbleSpawnRotation);

	float CurrentEffectiveness = PlayerPawn->GetAbilitySystemComponent()->GetNumericAttributeBase(UBubbleAttributeSet::GetEffectivenessAttribute());
	FlatBubble->GetAbilitySystemComponent()->SetNumericAttributeBase(UBubbleAttributeSet::GetEffectivenessAttribute(), CurrentEffectiveness);
	FlatBubble->HumanBubbleOwner = PlayerPawn;
}

void ASublevelTransitioner::InteractRequest(AController* InteractingCharacter)
{
	Super::InteractRequest(InteractingCharacter);

	if (HasAuthority() == false)
	{
		return;
	}

	ABubbleController* PlayerCont = Cast<ABubbleController>(InteractingCharacter);
	if (IsValid(PlayerCont) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ASublevelTransitioner::InteractRequest IsValid(PlayerCont) == false"));
		return;
	}

	AHumanBubble* PlayerPawn = Cast<AHumanBubble>(InteractingCharacter->GetPawn());
	if(IsValid(PlayerPawn) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ASublevelTransitioner::InteractRequest IsValid(PlayerPawn) == false"));
		return;
	}

	PlayerPawn->NetMulticast_PlayAnimationMontage(PlayerPawn->SeparateArmAnimation);
	PlayerCont->SetIsInputEnabled(false);

	FTimerHandle ContinueInteractionTimer;
	FTimerDelegate ContinueInteractionTimerDelegate;

	ContinueInteractionTimerDelegate.BindUFunction(this, FName("ContinueInteraction"), PlayerCont, PlayerPawn);
	GetWorldTimerManager().SetTimer(ContinueInteractionTimer, ContinueInteractionTimerDelegate, EntryTime, false);
	
}

bool ASublevelTransitioner::bCanInteract(AController* InteractingCharacter)
{
	return true;
}
