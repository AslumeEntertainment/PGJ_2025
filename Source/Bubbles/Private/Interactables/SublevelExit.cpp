// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/SublevelExit.h"

#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "GAS/BubbleAttributeSet.h"
#include "Camera/CameraActor.h"

#include "Characters/FlatBubbleCharacter.h"
#include "Characters/HumanBubble.h"
#include "UI/HUD/InGameHUD.h"

void ASublevelExit::InteractRequest(AController* InteractingCharacter)
{
	if (HasAuthority() == false)
	{
		return;
	}

	AFlatBubbleCharacter* FlatBubble = Cast<AFlatBubbleCharacter>(InteractingCharacter->GetPawn());
	if (IsValid(FlatBubble) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ASublevelExit::InteractRequest IsValid(FlatBubble) == false"));
		return;
	}

	APlayerController* PlayerCont = Cast<APlayerController>(InteractingCharacter);
	if (IsValid(PlayerCont) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ASublevelExit::InteractRequest IsValid(PlayerCont) == false"));
		return;
	}

	if (IsValid(FlatBubble->HumanBubbleOwner) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ASublevelExit::InteractRequest IsValid(FlatBubble->HumanBubbleOwner) == false"))
		return;
	}

	if (bIsExitSafe)
	{
		float CurrentEffectiveness = FlatBubble->GetAbilitySystemComponent()->GetNumericAttributeBase(UBubbleAttributeSet::GetEffectivenessAttribute());
		FlatBubble->HumanBubbleOwner->GetAbilitySystemComponent()->SetNumericAttributeBase(UBubbleAttributeSet::GetEffectivenessAttribute(), CurrentEffectiveness);
		FlatBubble->HumanBubbleOwner->UnbindAllInputBindings();

		//restore arm
	}

	AInGameHUD* HUD = Cast<AInGameHUD>(PlayerCont->GetHUD());
	if (IsValid(HUD))
	{
		HUD->ShowInteractionWidget();
	}

	//FlatBubble->UnbindAllInputBindings();
	PlayerCont->Possess(FlatBubble->HumanBubbleOwner);

	FlatBubble->Destroy();
}

bool ASublevelExit::bCanInteract(AController* InteractingCharacter)
{
	return true;
}
