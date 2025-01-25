// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/SublevelTransitioner.h"

#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "GAS/BubbleAttributeSet.h"
#include "Camera/CameraActor.h"

#include "Characters/FlatBubbleCharacter.h"
#include "Characters/HumanBubble.h"
#include "UI/HUD/InGameHUD.h"

void ASublevelTransitioner::InteractRequest(AController* InteractingCharacter)
{
	//play anim

	if (HasAuthority() == false)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ASublevelTransitioner::InteractRequest IsValid(World) == false"));
		return;
	}

	AHumanBubble* PlayerPawn = Cast<AHumanBubble>(InteractingCharacter->GetPawn());
	if(IsValid(PlayerPawn) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ASublevelTransitioner::InteractRequest IsValid(PlayerPawn) == false"));
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

	PlayerPawn->UnbindAllInputBindings();
	PlayerCont->Possess(FlatBubble);
	PlayerCont->SetViewTargetWithBlend(SublevelCamera);

	float CurrentEffectiveness = PlayerPawn->GetAbilitySystemComponent()->GetNumericAttributeBase(UBubbleAttributeSet::GetEffectivenessAttribute());
	FlatBubble->GetAbilitySystemComponent()->SetNumericAttributeBase(UBubbleAttributeSet::GetEffectivenessAttribute(), CurrentEffectiveness);
	FlatBubble->HumanBubbleOwner = PlayerPawn;

	AInGameHUD* HUD = Cast<AInGameHUD>(PlayerCont->GetHUD());
	if (IsValid(HUD))
	{
		HUD->ToggleInteractionWidget(false);
	}
}

bool ASublevelTransitioner::bCanInteract(AController* InteractingCharacter)
{
	return true;
}
