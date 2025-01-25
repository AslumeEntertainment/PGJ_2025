// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/PaintableItemSpawner.h"

#include "Interactables/PaintableItem.h"

// Sets default values
APaintableItemSpawner::APaintableItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

// Called when the game starts or when spawned
void APaintableItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority() == false)return;

	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APaintableItemSpawner::BeginPlay IsValid(World) == false"));
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	int NumSpawnPoints = SpawnPoints.Num();

	for (int i = 0; i < NumSpawnPoints; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Iteration - %d"), i);
		int RandIndex = i<MinimumSpawnedItems ? 0 : FMath::RandRange(0, SpawnableItems.Num() - 1);
		UE_LOG(LogTemp, Warning, TEXT("RandIndex - %d"), RandIndex);
		int RandIndexLocation = FMath::RandRange(0, SpawnPoints.Num() - 1);
		UE_LOG(LogTemp, Warning, TEXT("RandLocation - %d"), RandIndexLocation);
		FVector RandLocation = SpawnPoints[RandIndexLocation];
		SpawnPoints.RemoveAt(RandIndexLocation);
		UE_LOG(LogTemp, Warning, TEXT("After remove location lenghts - %d"), SpawnPoints.Num());
		if (SpawnableItems[RandIndex] == nullptr) continue;

		APaintableItem* Item = World->SpawnActor<APaintableItem>(SpawnableItems[RandIndex], RandLocation, FRotator(0, 0, 0), Params);
		SpawnedItems.Add(Item);
		Item->OnCleannessUpdated.AddDynamic(this, &APaintableItemSpawner::OnNetWorthChanged);
		TotalNetWorth += Item->GetNetWorth();
	}

}

void APaintableItemSpawner::OnNetWorthChanged()
{
	Cleaner = 0;
	Contaminator = 0;

	for (APaintableItem* Item : SpawnedItems)
	{
		int Points = Item->GetActualPoints();
		Points < 0 ? Contaminator += Points : Cleaner += Points;
	}

	OnCleanerPointsChanged.Broadcast(Cleaner);
	OnContaminatorPointsChanged.Broadcast(Contaminator);
	OnProgrssUpdated.Broadcast((Cleaner + Contaminator + TotalNetWorth) / (TotalNetWorth * 2));
}
