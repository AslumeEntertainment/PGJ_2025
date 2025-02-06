// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/ItemBase.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(Root);
}

void AItemBase::InteractRequest(AController* InteractingCharacter)
{
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

