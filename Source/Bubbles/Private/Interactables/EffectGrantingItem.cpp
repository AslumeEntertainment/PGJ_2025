// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/EffectGrantingItem.h"

#include "GameFramework/RotatingMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"

AEffectGrantingItem::AEffectGrantingItem()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	RotationComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
}

void AEffectGrantingItem::BeginPlay()
{
	Super::BeginPlay();
	RotationComponent->RotationRate.Yaw = RotationRate;
}

void AEffectGrantingItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BaseMesh->AddRelativeLocation(FVector(0, 0, FMath::Sin(GetGameTimeSinceCreation())*FloatingHeighOffsetMultiplier));
}

void AEffectGrantingItem::InteractRequest(AController* InteractingCharacter)
{
	if (HasAuthority() == false)
	{
		return;
	}
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InteractingCharacter->GetPawn());

	if (IsValid(ASC) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AEffectGrantingItem::InteractRequest - No valid ASC"));
		return;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> EffectClass : Effects)
	{

		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EffectClass, 0, EffectContext);
		ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	}

	NetMulticast_OnCollected();
	Destroy();
}

void AEffectGrantingItem::NetMulticast_OnCollected_Implementation()
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AFlatBubbleCharacter::Pop_Implementation IsValid(World) == false"));
		return;
	}
	if (IsValid(OnCollectedEffect) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("AFlatBubbleCharacter::Pop_Implementation IsValid(OnCollectedEffect) == false"));
		return;
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, OnCollectedEffect, GetActorLocation(), GetActorRotation());
}
