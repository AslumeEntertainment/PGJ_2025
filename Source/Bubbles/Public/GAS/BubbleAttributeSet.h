// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BubbleAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) 

UCLASS()
class BUBBLES_API UBubbleAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;


	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Effectiveness)
	FGameplayAttributeData Effectiveness;
	ATTRIBUTE_ACCESSORS(UBubbleAttributeSet, Effectiveness);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxEffectiveness)
	FGameplayAttributeData MaxEffectiveness;
	ATTRIBUTE_ACCESSORS(UBubbleAttributeSet, MaxEffectiveness);

	bool HasMaxEffectiveness() const;

protected:

	UFUNCTION()
	virtual void OnRep_Effectiveness(const FGameplayAttributeData& OldEffectiveness);

	UFUNCTION()
	virtual void OnRep_MaxEffectiveness(const FGameplayAttributeData& OldMaxEffectiveness);

};