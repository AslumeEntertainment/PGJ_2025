// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/PaintableItem.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"

#include "GAS/BubbleAttributeSet.h"

void APaintableItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(APaintableItem, Cleanness, COND_None, REPNOTIFY_Always);
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

	

	UpdateTexture();
}

void APaintableItem::OnRep_Cleanness()
{
	UpdateTexture();
}

void APaintableItem::UpdateTexture()
{
	/*switch (Cleanness)
	{
	default:
		break;//update textures
	}*/
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
	UE_LOG(LogTemp, Warning, TEXT("Cleaning - Cleanness: %d  Step:%d"), Cleanness, Iterations);

	if ((bCanInteract(InteractingPlayer)==false ) || Iterations<=0)
	{
		InteractingPlayer->IsLocalPlayerController() ? StopInteraction() : Client_StopInteracting();
	}
}

void APaintableItem::Client_StopInteracting_Implementation()
{
	StopInteraction();
}

void APaintableItem::StopInteraction()
{
	InteractingPlayer->SetInputMode(FInputModeGameOnly());
	InteractingPlayer = nullptr;
	GetWorldTimerManager().ClearTimer(CleaningPeriodTimer);
	CleaningPeriodTimer.Invalidate();

	UE_LOG(LogTemp, Warning, TEXT("Stopping clean - Cleanness: %d Step:%d"), Cleanness, Iterations);
}

void APaintableItem::InteractRequest(AController* InteractingCharacter)
{

	InteractingPlayer = Cast<APlayerController>(InteractingCharacter);
	InteractingPlayer->SetInputMode(FInputModeUIOnly());

	if (HasAuthority() == false)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Starting Clean - Cleanness: %d Step:%d"), Cleanness, Iterations);
	

	Iterations = CleaningTime;

	GetWorldTimerManager().SetTimer(CleaningPeriodTimer, this, &APaintableItem::ProgressCleaning, CleaningInterval, true, CleaningInterval);

}

bool APaintableItem::bCanInteract(AController* InteractingCharacter)
{
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

