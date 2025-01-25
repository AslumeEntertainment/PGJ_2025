// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GameplayTagContainer.h"

#include "Headers/GeneralDelegates.h"

#include "BubbleCharacter.generated.h"

class UMaterialInterface;
class UAbilitySystemComponent;
class UBubbleAttributeSet;

UCLASS(Abstract)
class BUBBLES_API ABubbleCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABubbleCharacter();

private:

	UPROPERTY(Replicated)
	UObject* FocusedInteractableObject = nullptr;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	UMaterialInterface* InteractableOverlayMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionRange = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	FGameplayTag InteractionAbilityTag;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bShouldDrawDebug = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(Transient)
	UBubbleAttributeSet* AttributeSet;

	UFUNCTION(Server, Reliable)
	void Server_SetFocusedInteractable(UObject* InFocusedInteractable);

	void SetFocusedInteractable(UObject* InFocusedInteractable);

	virtual void EmitInteractionChecker();

	void CheckForInteractables(FHitResult HitResult);


	void TriggerInteraction();

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FTextTransferSignature InteractIndicationTextDelegate;

};
