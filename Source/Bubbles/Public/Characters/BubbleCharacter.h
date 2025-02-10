// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"

#include "Headers/GeneralDelegates.h"

#include "BubbleCharacter.generated.h"

class IInteractable;
class UMaterialInterface;
class UAbilitySystemComponent;
class UBubbleAttributeSet;
class UInputMappingContext;
class UInputAction;
class UNiagaraSystem;
struct FInputActionValue;


UCLASS(Abstract)
class BUBBLES_API ABubbleCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABubbleCharacter();

private:

	bool bHasBeenInited = false;

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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS|CharacterDefaults")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS|CharacterDefaults")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(Transient)
	UBubbleAttributeSet* AttributeSet;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UFUNCTION(Server, Reliable)
	void Server_SetFocusedInteractable(UObject* InFocusedInteractable);

	void SetFocusedInteractable(UObject* InFocusedInteractable);

	virtual void EmitInteractionChecker();

	bool CheckForInteractables(FHitResult HitResult);

	void TriggerInteraction();

public:

	UFUNCTION()
	void UpdateInteractionText(FText InteractableName, bool bCanInteract);

	void InitCharacterDefaults();

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Client, Reliable)
	void Client_BindMappingContext();

	UFUNCTION(Client, Reliable)
	void Client_UnbindMappingContext();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_ShowEffectAtCharacterLocation(UNiagaraSystem* NiagaraEffect);

	UFUNCTION()
	void RotateTowardsActor(UWorld* World, AActor* TargetActor);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_PlayAnimationMontage(UAnimMontage* Animation);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_StopAnimationMontage(UAnimMontage* Animation);

	UFUNCTION()
	virtual void Move(const FInputActionValue& Value);

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };

	UObject* GetFocusedInteractableObject() { return FocusedInteractableObject; }

	UPROPERTY(BlueprintAssignable)
	FTextTransferSignature InteractIndicationTextDelegate;
};
