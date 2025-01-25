// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Headers/GeneralDelegates.h"

#include "BubbleCharacter.generated.h"

class UMaterialInterface;

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

	UFUNCTION(Server, Reliable)
	void Server_SetFocusedInteractable(UObject* InFocusedInteractable);

	void SetFocusedInteractable(UObject* InFocusedInteractable);

	virtual void EmitInteractionChecker();
	
	void CheckForInteractables(FHitResult HitResult);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	UMaterialInterface* InteractableOverlayMaterial;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FTextTransferSignature InteractIndicationTextDelegate;

};
