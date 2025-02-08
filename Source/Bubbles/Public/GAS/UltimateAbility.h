// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "GameplayTagContainer.h"

#include "UltimateAbility.generated.h"

class AHumanBubble;
class UNiagaraSystem;
class UUltimateAbilityWidget;

UCLASS()
class BUBBLES_API UUltimateAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	AHumanBubble* Player;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityProperties")
	float AbilityRadius = 100;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityProperties")
	FGameplayTag ContinueAbilityTag;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UAnimMontage* AbilityAnimationMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UNiagaraSystem* AbilityNiagaraEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals|UI");
	TSubclassOf<UUltimateAbilityWidget> AbilityWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals|UI");
	FText AbilityWidgetTopText;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals|UI");
	FText AbilityWidgetBottomText;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals|UI");
	FLinearColor AbilityWidgetTextColor;

	UFUNCTION()
	void OnAnimMontageCompleted();

	UFUNCTION()
	void OnEventReceived(FGameplayEventData Payload);

};
