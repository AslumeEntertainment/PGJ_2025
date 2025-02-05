
#include "GAS/BubbleAttributeSet.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

#include "Characters/BubbleCharacter.h"
//#include "GAS/SG_AbilitySystemComponent.h"


void UBubbleAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBubbleAttributeSet, Effectiveness, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBubbleAttributeSet, MaxEffectiveness, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBubbleAttributeSet, Energy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBubbleAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
}

void UBubbleAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	/*USG_AbilitySystemComponent* ASC = Cast<USG_AbilitySystemComponent>(GetOwningAbilitySystemComponent());
	if (IsValid(ASC) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("USG_AttributeSet::PostAttributeBaseChange IsValid(ASC) == false"));
		return;
	}

	if (Attribute == GetHealthAttribute())
	{
		UHittableComponent* HittableComp = GetOwningActor()->GetComponentByClass<UHittableComponent>();
		if (IsValid(HittableComp) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("USG_AttributeSet::PostAttributeBaseChange IsValid(HittableComp) == false"));
			return;
		}

		//UE_LOG(LogTemp, Warning, TEXT("USG_AttributeSet: %s now has %.2f out of %.2f health"), *GetOwningActor()->GetName(), GetHealth(), GetMaxHealth())

		if (HasMaxHealth())
		{
			ASC->AddReplicatedLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Bool.HasFull.Health")));
			ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Bool.HasFull.Health")));
		}
		else if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Bool.HasFull.Health"))))
		{
			ASC->RemoveReplicatedLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Bool.HasFull.Health")));
			ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Bool.HasFull.Health")));
		}

		if (GetHealth() == 0.f)
		{
			ASC->AddReplicatedLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Bool.HasNone.Health")));
			ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Bool.HasNone.Health")));
			HittableComp->OnHittableDestroyed();
		}
	}*/
}


bool UBubbleAttributeSet::HasMaxEffectiveness() const
{
	return GetEffectiveness() == GetMaxEffectiveness();
}

void UBubbleAttributeSet::OnRep_Effectiveness(const FGameplayAttributeData& OldEffectiveness)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBubbleAttributeSet, Effectiveness, OldEffectiveness);
}

void UBubbleAttributeSet::OnRep_MaxEffectiveness(const FGameplayAttributeData& OldMaxEffectiveness)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBubbleAttributeSet, MaxEffectiveness, OldMaxEffectiveness);
}

void UBubbleAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBubbleAttributeSet, Energy, OldEnergy);
}

void UBubbleAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBubbleAttributeSet, MaxEnergy, OldMaxEnergy);
}
