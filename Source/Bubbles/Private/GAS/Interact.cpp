// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Interact.h"

#include "Characters/BubbleCharacter.h"
#include "Interactables/Interactable.h"


void UInteract::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Error, TEXT("USGGA_Interact::ActivateAbility CommitAbility == false"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
	ABubbleCharacter* Player = Cast<ABubbleCharacter>(ActorInfo->AvatarActor.Get());
	if (!IsValid(Player))
	{
		UE_LOG(LogTemp, Error, TEXT("USGGA_Interact::ActivateAbility IsValid(Player) == false"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	IInteractable* FocusedInteractable = Cast<IInteractable>(Player->GetFocusedInteractableObject());
	if (FocusedInteractable == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("USGGA_Interact::ActivateAbility FocusedInteractable == nullptr"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (FocusedInteractable->bCanInteract(Player->GetController()) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s cannot interact with current focused interactable!"), *Player->GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	FocusedInteractable->InteractRequest(Player->GetController());

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}