// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/SublevelExit.h"

#include "GameFramework/PlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAS/BubbleAttributeSet.h"
#include "Camera/CameraActor.h"

#include "Characters/FlatBubbleCharacter.h"
#include "Characters/HumanBubble.h"
#include "BubbleController.h"
#include "UI/HUD/InGameHUD.h"

void ASublevelExit::ContinueInteraction()
{
}

void ASublevelExit::InteractRequest(AController* InteractingCharacter)
{
	if (HasAuthority() == false)
	{
		return;
	}

	ABubbleController* PlayerCont = Cast<ABubbleController>(InteractingCharacter);
	if (IsValid(PlayerCont) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ASublevelExit::InteractRequest IsValid(PlayerCont) == false"));
		return;
	}

	AFlatBubbleCharacter* FlatBubble = Cast<AFlatBubbleCharacter>(InteractingCharacter->GetPawn());
	if (IsValid(FlatBubble) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ASublevelExit::InteractRequest IsValid(FlatBubble) == false"));
		return;
	}

	if (IsValid(FlatBubble->HumanBubbleOwner) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ASublevelExit::InteractRequest IsValid(FlatBubble->HumanBubbleOwner) == false"))
		return;
	}

	if (bIsSafeExit)
	{
		float CurrentEffectiveness = FlatBubble->GetAbilitySystemComponent()->GetNumericAttributeBase(UBubbleAttributeSet::GetEffectivenessAttribute());
		FlatBubble->HumanBubbleOwner->GetAbilitySystemComponent()->SetNumericAttributeBase(UBubbleAttributeSet::GetEffectivenessAttribute(), CurrentEffectiveness);

		FlatBubble->HumanBubbleOwner->NetMulticast_PlayAnimationMontage(FlatBubble->HumanBubbleOwner->NaturalRegrowAnimation);
	}
	else
	{
		//pop
	}

	FlatBubble->Client_UnbindMappingContext();
	PlayerCont->Possess(FlatBubble->HumanBubbleOwner);

	FlatBubble->Destroy();
}

bool ASublevelExit::bCanInteract(AController* InteractingCharacter)
{
	return true;
}
