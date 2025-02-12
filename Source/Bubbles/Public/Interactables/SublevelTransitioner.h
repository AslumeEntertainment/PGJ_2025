// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/ItemBase.h"
#include "SublevelTransitioner.generated.h"


class AHumanBubble;
class ABubbleController;
class AFlatBubbleCharacter;

UCLASS()
class BUBBLES_API ASublevelTransitioner : public AItemBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "SublevelInfo")
	float EntryTime = 0.f;

	UPROPERTY(EditAnywhere, Category = "SublevelInfo")
	FVector SublevelStartingLocation;

	UPROPERTY(EditAnywhere, Category = "SublevelInfo")
	FRotator FlatBubbleSpawnRotation;

	UPROPERTY(EditInstanceOnly, Category = "SublevelInfo")
	ACameraActor* SublevelCamera = nullptr;

	UFUNCTION()
	void ContinueInteraction(ABubbleController* PlayerCont, AHumanBubble* PlayerPawn);

public:

	virtual void InteractRequest(AController* InteractingCharacter) override;

	virtual bool bCanInteract(AController* InteractingCharacter) override;

};
