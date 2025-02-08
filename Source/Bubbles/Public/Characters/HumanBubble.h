// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BubbleCharacter.h"

#include "Headers/GeneralDelegates.h"

#include "HumanBubble.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(config=Game)
class AHumanBubble : public ABubbleCharacter
{
	GENERATED_BODY()

	bool bAreAttributesBoundToUI = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AbilityAction;

	UPROPERTY(EditDefaultsOnly, Category = "CustomValues|Ability")
	FGameplayTagContainer AbilityTag;

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_OnEffectivenessUpdated(float CurrentEffectiveness, float MaxEffectiveness);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_OnEnergyUpdated(float CurrentEnergy, float MaxEnergy);

public:

	AHumanBubble();

protected:

	virtual void PossessedBy(AController* NewController) override;

	virtual void Move(const FInputActionValue& Value) override;

	void Look(const FInputActionValue& Value);	
	
	virtual void Tick(float DeltaTime) override;

	virtual void EmitInteractionChecker() override;

	void TriggerAbility();

public:

	UPROPERTY(EditDefaultsOnly, Category = "CustomValues|Animation")
	UMaterialInterface* FlatBubbleMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "CustomValues|Animation")
	UAnimMontage* CleanAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "CustomValues|Animation")
	UAnimMontage* InteractAnimation;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_PlayCharacterAnimation(UAnimMontage* Animation, bool Looping);

	FFloatTransferSignature OnEffectivenessUpdated;
	FFloatTransferSignature OnEnergyUpdated;

};

