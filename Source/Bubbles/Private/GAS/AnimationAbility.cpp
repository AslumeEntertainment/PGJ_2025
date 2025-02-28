// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AnimationAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


void UAnimationAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CommitAbility(Handle, ActorInfo, ActivationInfo) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("UInflateArm::ActivateAbility CommitAbility == false"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AbilityAnimationMontage);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UAnimationAbility::OnAnimMontageCompleted);
	PlayMontageTask->ReadyForActivation();
}

void UAnimationAbility::OnAnimMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
