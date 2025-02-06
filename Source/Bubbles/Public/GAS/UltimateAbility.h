// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "GameplayTagContainer.h"

#include "UltimateAbility.generated.h"

class AHumanBubble;

UCLASS()
class BUBBLES_API UUltimateAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

protected:

	AHumanBubble* Player;

	UPROPERTY(EditDefaultsOnly)
	float AbilityRadius = 100;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AbilityAnimationMontage;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ContinueAbilityTag;

	UPROPERTY(EditDefaultsOnly);
	TSubclassOf<UUserWidget> AbilityWidgetClass;

	UFUNCTION()
	void OnAnimMontageCompleted();

	UFUNCTION()
	void OnEventReceived(FGameplayEventData Payload);
};
