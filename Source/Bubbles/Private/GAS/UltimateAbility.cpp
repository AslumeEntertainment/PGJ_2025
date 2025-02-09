// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/UltimateAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "UI/Widgets/UltimateAbilityWidget.h"
#include "Interactables/PaintableItem.h"
#include "Characters/HumanBubble.h"
#include "GAS/BubbleAttributeSet.h"


void UUltimateAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CommitAbility(Handle, ActorInfo, ActivationInfo) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("UUltimateAbility::ActivateAbility CommitAbility == false"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	Player = Cast<AHumanBubble>(ActorInfo->AvatarActor.Get());
	if (IsValid(Player) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("UUltimateAbility::ActivateAbility IsValid(Player) == false"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (IsLocallyControlled())
	{
		UWorld* World = GetWorld();
		if (IsValid(World) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("UUltimateAbility::ActivateAbility IsValid(World) == false"));
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}

		UUltimateAbilityWidget* AbilityWidget = CreateWidget<UUltimateAbilityWidget>(UGameplayStatics::GetGameInstance(World), AbilityWidgetClass);
		if (IsValid(AbilityWidget))
		{
			AbilityWidget->SetUltimateAbilityText(AbilityWidgetTopText, AbilityWidgetBottomText, AbilityWidgetTextColor);
			AbilityWidget->AddToViewport();
		}
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AbilityAnimationMontage);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UUltimateAbility::OnAnimMontageCompleted);
	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ContinueAbilityTag, nullptr, true, true);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &UUltimateAbility::OnEventReceived);
	WaitGameplayEventTask->ReadyForActivation();
}

void UUltimateAbility::OnAnimMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UUltimateAbility::OnEventReceived(FGameplayEventData Payload)
{
	if (HasAuthority(&CurrentActivationInfo) == false)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("UUltimateAbility::OnEventReceived IsValid(World) == false"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	Player->NetMulticast_ShowEffectAtCharacterLocation(AbilityNiagaraEffect);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;

	if(UKismetSystemLibrary::SphereOverlapActors(World, Player->GetActorLocation(), AbilityRadius, ObjectTypes, APaintableItem::StaticClass(), 
		ActorsToIgnore, OutActors) == false) return;

	float Effectiveness = Player->GetAbilitySystemComponent()->GetNumericAttributeBase(UBubbleAttributeSet::GetEffectivenessAttribute());

	for (AActor* Actor : OutActors)
	{
		APaintableItem* PaintableItem = Cast<APaintableItem>(Actor);
		if (IsValid(PaintableItem) == false)
		{
			continue;
		}

		int CleannesToAdd = FMath::TruncToInt(PaintableItem->GetMaxCleanness()* FMath::Sign(Effectiveness));
		PaintableItem->SetCleanness(PaintableItem->GetCurrentCleanness() + CleannesToAdd);
	}
}
