// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Headers/GeneralDelegates.h"
#include "PaintableItemSpawner.generated.h"

class APaintableItem;

UCLASS()
class BUBBLES_API APaintableItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APaintableItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<APaintableItem>> SpawnableItems;

	UPROPERTY(EditAnywhere)
	TArray<FVector> SpawnPoints;

	UPROPERTY(EditAnywhere)
	int MinimumSpawnedItems;

	UPROPERTY(VisibleAnywhere)
	TArray<APaintableItem*> SpawnedItems;

	int Cleaner = 0;
	int Contaminator = 0;
	int TotalNetWorth = 0;

	UFUNCTION()
	void OnNetWorthChanged();
public:

	int GetWinner() { return Cleaner + Contaminator; }

	FIntegerTransferSignature OnCleanerPointsChanged;
	FIntegerTransferSignature OnContaminatorPointsChanged;
	FFloatTransferSignature OnProgrssUpdated;
};
