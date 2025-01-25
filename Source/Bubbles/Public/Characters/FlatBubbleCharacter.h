// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BubbleCharacter.h"
#include "FlatBubbleCharacter.generated.h"

class UCapsuleComponent;

UCLASS()
class BUBBLES_API AFlatBubbleCharacter : public ABubbleCharacter
{
	GENERATED_BODY()

public:

	AFlatBubbleCharacter();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* InteractionCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Move(const FInputActionValue& Value) override;

	void StartCrouch();

	void StopCrouch();

	UFUNCTION()
	void OnInteractionCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};