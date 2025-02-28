// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/ItemBase.h"

#include "Components/StaticMeshComponent.h"

#include "Characters/HumanBubble.h"
#include "BubbleController.h"

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
	ABubbleController* BubbleController = Cast<ABubbleController>(InteractingCharacter);
	if (IsValid(BubbleController) == false) 
	{
		UE_LOG(LogTemp, Error, TEXT("AItemBase::InteractRequest IsValid(BubbleController) == false"));
		return;
	}

	AHumanBubble* PlayerPawn = Cast<AHumanBubble>(InteractingCharacter->GetPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->RotateTowardsActor(GetWorld(), this);
		PlayerPawn->PointCameraTowardsActor(GetWorld(), this, BubbleController);
	}
}

bool AItemBase::bCanInteract(AController* InteractingCharacter)
{
	return true;
}

FText AItemBase::GetInteractableName()
{
	return ItemName;
}

