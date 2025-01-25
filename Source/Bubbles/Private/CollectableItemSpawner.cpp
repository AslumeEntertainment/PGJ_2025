// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableItemSpawner.h"

#include "Interactables/EffectGrantingItem.h"

// Sets default values
ACollectableItemSpawner::ACollectableItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACollectableItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority() == false)return;

	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ACollectableItemSpawner::BeginPlay IsValid(World) == false"));
		return;
	}

	int NumItemVariations = SpawnableItems.Num();
	int NumSpawnPoints = SpawnPoints.Num();

	if (NumSpawnPoints % NumItemVariations != 0){
		UE_LOG(LogTemp, Error, TEXT("ACollectableItemSpawner::BeginPlay - NumSpawnPoints mod NumItemVariations != 0"));
		return;
	}
	int ItemPairsToSpawn = FMath::RandRange(0, NumSpawnPoints / NumItemVariations);

	for (int i = 0; i < ItemPairsToSpawn; i++)
	{
		for (int j = 0; j < NumItemVariations; j++)
		{
			int RandIndex = FMath::RandRange(0, SpawnPoints.Num()-1);

			FActorSpawnParameters Params;
			World->SpawnActor<AEffectGrantingItem>(SpawnableItems[j], SpawnPoints[RandIndex], ItemOrientation, FActorSpawnParameters());

			SpawnPoints.RemoveAt(RandIndex);
		}
	}
	
}

