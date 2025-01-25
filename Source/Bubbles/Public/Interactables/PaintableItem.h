// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/ItemBase.h"
#include "Headers/GeneralDelegates.h"	
#include "PaintableItem.generated.h"

class ABubbleController;

UCLASS()
class BUBBLES_API APaintableItem : public AItemBase
{
	GENERATED_BODY()
	
public:
	APaintableItem();

private:
	UPROPERTY()
	ABubbleController* InteractingPlayer;

	UPROPERTY()
	int Iterations = 0;

protected:

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Cleanness, VisibleAnywhere)
	int Cleanness = 0;

	UPROPERTY(EditAnywhere)
	int MaxCleanness = 10;

	UPROPERTY(EditAnywhere)
	int CleannessShield = 2;

	UPROPERTY(EditAnywhere)
	int PointMultiplier = 2;

	UPROPERTY(EditAnywhere)
	float CleaningTime = 5;

	UPROPERTY(EditAnywhere)
	float CleaningInterval = 1;

	UPROPERTY(Replicated)
	bool IsLocked = false;

	FTimerHandle CleaningPeriodTimer;

	void SetCleanness(int NewValue, bool bCanBypass = false);

	UFUNCTION()
	void OnRep_Cleanness();

	void UpdateTexture();

	void ProgressCleaning();

	void StopInteraction();

	virtual void InteractRequest(AController* InteractingCharacter) override;

	virtual bool bCanInteract(AController* InteractingCharacter) override;
public:
	int GetNetWorth();
	int GetActualPoints();


	FVoidDataTransferSignature OnCleannessUpdated;
};
