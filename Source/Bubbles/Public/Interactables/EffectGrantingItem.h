// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/ItemBase.h"

#include "EffectGrantingItem.generated.h"

class UGameplayEffect;

UCLASS()
class BUBBLES_API AEffectGrantingItem : public AItemBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	virtual void InteractRequest(AController* InteractingCharacter) override;

public:

	TArray<TSubclassOf<UGameplayEffect>> GetEffects() { return Effects; }

};
