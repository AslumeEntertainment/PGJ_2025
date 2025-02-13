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

class AFlatBubbleCharacter;

UCLASS(config=Game)
class AHumanBubble : public ABubbleCharacter
{
	GENERATED_BODY()

public:

	AHumanBubble();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UltimateAbilityAction;

	UPROPERTY(EditDefaultsOnly, Category = "CustomValues|Ability")
	FGameplayTagContainer UltimateAbilityTags;

	UPROPERTY(EditDefaultsOnly, Category = "CustomValues|Ability")
	FGameplayTagContainer InflateArmAbilityTags;

	UPROPERTY(Replicated)
	bool bIsArmless = false;

	virtual void PossessedBy(AController* NewController) override;

	virtual void BindCallbacksToDependencies() override;

	virtual void Move(const FInputActionValue& Value) override;

	void Look(const FInputActionValue& Value);	
	
	virtual void Tick(float DeltaTime) override;

	virtual void EmitInteractionChecker() override;

	void TriggerInteraction() override;

	void TriggerUltimateAbility();

public:

	UPROPERTY(EditAnywhere, Category = "CustomValues")
	TSubclassOf<AFlatBubbleCharacter> FlatBubbleClass;

	UPROPERTY(EditDefaultsOnly, Category = "CustomValues|Animation")
	UAnimMontage* CleanAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "CustomValues|Animation")
	UAnimMontage* SeparateArmAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "CustomValues|Animation")
	UAnimMontage* NaturalRegrowAnimation;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void PointCameraTowardsActor(UWorld* World, AActor* TargetActor, APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void SetIsArmless(bool NewValue);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsArmless() const { return bIsArmless; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};

