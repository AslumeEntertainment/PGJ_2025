// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/PaintableItem.h"

#include "Components/StaticMeshComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Net/UnrealNetwork.h"

#include "GAS/BubbleAttributeSet.h"
#include "Characters/HumanBubble.h"
#include "BubbleController.h"


void APaintableItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(APaintableItem, Cleanness, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(APaintableItem, IsLocked, COND_None, REPNOTIFY_Always);
}

APaintableItem::APaintableItem()
{
	bReplicates = true;

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetupAttachment(GetRootComponent());
}

void APaintableItem::SetCleanness(int NewValue, bool bCanBypass)
{
	int PrevCleanness = Cleanness;
	
	Cleanness = NewValue;

	if (FMath::Abs(PrevCleanness) < FMath::Abs(Cleanness))
	{
		Cleanness = FMath::Clamp(NewValue, -MaxCleanness, MaxCleanness);
		if (FMath::Abs(Cleanness) == MaxCleanness)
		{
			Cleanness += FMath::Sign(Cleanness) * CleannessShield;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Cleaning - Cleanness: %d"), Cleanness);

	OnCleannessUpdated.Broadcast();
	UpdateTexture();
}

void APaintableItem::OnRep_Cleanness()
{
	OnCleannessUpdated.Broadcast();
	UpdateTexture();
}

void APaintableItem::UpdateTexture()
{
	UMaterialInstanceDynamic* DynamicMaterial = BaseMesh->CreateDynamicMaterialInstance(0);
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue("Cleanness", GetCurrentCleannessPercent());
	}

	UMaterialInstanceDynamic* ShieldDynamicMaterial = ShieldMesh->CreateDynamicMaterialInstance(0);
	if (ShieldDynamicMaterial)
	{
		if (GetCurrentCleannessPercent() >= 1.f || GetCurrentCleannessPercent() <= 0.f)
		{
			ShieldDynamicMaterial->SetScalarParameterValue("Sign", FMath::Sign(Cleanness));
		}
		else
		{
			ShieldDynamicMaterial->SetScalarParameterValue("Sign", 0);
		}
	}
}

void APaintableItem::NetMulticast_ShowEffect_Implementation(UNiagaraSystem* CleannessEffect)
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APaintableItem::ShowEffect IsValid(World) == false"));
		return;
	}
	if (IsValid(CleannessEffect) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("APaintableItem::ShowEffect IsValid(CleannessEffect) == false"));
		return;
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, CleannessEffect, GetActorLocation(), GetActorRotation());
}

void APaintableItem::ProgressCleaning()
{
	Iterations-= CleaningInterval;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InteractingPlayer->GetPawn());

	if (IsValid(ASC) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APaintableItem::ProgressCleaning - No valid ASC"));
		return;
	}

	const UBubbleAttributeSet* AttributeSet = Cast<UBubbleAttributeSet>(ASC->GetAttributeSet(UBubbleAttributeSet::StaticClass()));

	SetCleanness(Cleanness + AttributeSet->GetEffectiveness());

	if (AttributeSet->GetEffectiveness() > 0) NetMulticast_ShowEffect(CleannessUpEffect);
	else if (AttributeSet->GetEffectiveness() < 0) NetMulticast_ShowEffect(CleannessDownEffect);

	if ((FMath::Abs(Cleanness) >= MaxCleanness && FMath::Sign(AttributeSet->GetEffectiveness()) == FMath::Sign(Cleanness)) )
	{
		IsLocked = false;
		StopInteraction(true);
		return;
	}
	if (Iterations <= 0)
	{
		IsLocked = false;
		StopInteraction();
		return;
	}
}

void APaintableItem::StopInteraction(bool GiveEnregy)
{
	if (GiveEnregy)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InteractingPlayer->GetPawn());
		const UBubbleAttributeSet* AttributeSet = Cast<UBubbleAttributeSet>(ASC->GetAttributeSet(UBubbleAttributeSet::StaticClass()));

		ASC->SetNumericAttributeBase(UBubbleAttributeSet::GetEnergyAttribute(), FMath::Clamp(AttributeSet->GetEnergy() + GetNetWorth(), 0, AttributeSet->GetMaxEnergy()));
	}

	AHumanBubble* PlayerPawn = Cast<AHumanBubble>(InteractingPlayer->GetPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->NetMulticast_StopAnimationMontage(PlayerPawn->CleanAnimation);
	}

	InteractingPlayer->Client_SetInputMode(EInputMode::GameOnly);
	InteractingPlayer = nullptr;
	GetWorldTimerManager().ClearTimer(CleaningPeriodTimer);
	CleaningPeriodTimer.Invalidate();

	UE_LOG(LogTemp, Warning, TEXT("Stopping clean - Cleanness: %d Step:%d"), Cleanness, Iterations);
}

void APaintableItem::InteractRequest(AController* InteractingCharacter)
{
	Super::InteractRequest(InteractingCharacter);
	if (HasAuthority() == false)
	{
		return;
	}

	InteractingPlayer = Cast<ABubbleController>(InteractingCharacter);
	InteractingPlayer->Client_SetInputMode(EInputMode::UIOnly);
	InteractingPlayer->StopMovement();

	AHumanBubble* PlayerPawn = Cast<AHumanBubble>(InteractingCharacter->GetPawn());
	if (IsValid(PlayerPawn))
	{
		PlayerPawn->NetMulticast_PlayAnimationMontage(PlayerPawn->CleanAnimation);
	}

	IsLocked = true;
	UE_LOG(LogTemp, Warning, TEXT("Starting Clean - Cleanness: %d Step:%d"), Cleanness, Iterations);
	
	Iterations = CleaningTime;

	GetWorldTimerManager().SetTimer(CleaningPeriodTimer, this, &APaintableItem::ProgressCleaning, CleaningInterval, true, CleaningInterval);

}

bool APaintableItem::bCanInteract(AController* InteractingCharacter)
{
	if (IsLocked) return false;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InteractingCharacter->GetPawn());

	if (IsValid(ASC) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("APaintableItem::bCanInteract - No valid ASC"));
		return false;
	}

	const UBubbleAttributeSet* AttributeSet = Cast<UBubbleAttributeSet>(ASC->GetAttributeSet(UBubbleAttributeSet::StaticClass()));

	if (AttributeSet->GetEffectiveness() == 0) return false;

	return !(FMath::Abs(Cleanness) >= MaxCleanness && FMath::Sign(AttributeSet->GetEffectiveness()) == FMath::Sign(Cleanness));
}

int APaintableItem::GetNetWorth()
{
	return PointMultiplier;
}

int APaintableItem::GetActualPoints()
{
	if (FMath::Abs(Cleanness) < MaxCleanness) return 0;
	return FMath::Sign(Cleanness) * PointMultiplier;
}

FText APaintableItem::GetInteractableName()
{
	FString InteractMessage = ItemName.ToString() + " - " + FString::FromInt(GetCurrentCleannessPercent() * 100) + "%";
	return FText::FromString(InteractMessage);
}

float APaintableItem::GetCurrentCleannessPercent()
{
	int TrueCleanness = FMath::Clamp(Cleanness, -MaxCleanness, MaxCleanness);

	return float(TrueCleanness + MaxCleanness) / float(2 * MaxCleanness);
}

