// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/ItemBase.h"

#include "Components/StaticMeshComponent.h"

#include "Characters/HumanBubble.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(Root);
}

void AItemBase::InteractRequest(AController* InteractingCharacter)
{
	AHumanBubble* PlayerPawn = Cast<AHumanBubble>(InteractingCharacter->GetPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->RotateTowardsActor(GetWorld(), this);
	}
	UE_LOG(LogTemp, Display, TEXT("Interacted with: %s"), *ItemName.ToString());
}

bool AItemBase::bCanInteract(AController* InteractingCharacter)
{
	return true;
}

FText AItemBase::GetInteractableName()
{
	return ItemName;
}

