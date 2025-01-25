// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectableItemSpawner.generated.h"

class AEffectGrantingItem;

UCLASS()
class BUBBLES_API ACollectableItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectableItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AEffectGrantingItem>> SpawnableItems;

	UPROPERTY(EditAnywhere)
	TArray<FVector> SpawnPoints;

	UPROPERTY(EditAnywhere)
	FRotator ItemOrientation;

public:

};
