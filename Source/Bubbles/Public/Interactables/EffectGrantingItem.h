// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/ItemBase.h"

#include "EffectGrantingItem.generated.h"

class UGameplayEffect;
class URotatingMovementComponent;

UCLASS()
class BUBBLES_API AEffectGrantingItem : public AItemBase
{
	GENERATED_BODY()
	
public:

	AEffectGrantingItem();

private:

	float FloatingHeighOffsetMultiplier = 0.1;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	UPROPERTY(EditDefaultsOnly, Category = "Idle")
	float RotationRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Idle")
	URotatingMovementComponent* RotationComponent;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void InteractRequest(AController* InteractingCharacter) override;

public:

	TArray<TSubclassOf<UGameplayEffect>> GetEffects() { return Effects; }

};
