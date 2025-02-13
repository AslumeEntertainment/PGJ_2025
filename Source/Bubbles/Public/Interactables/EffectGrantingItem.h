// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/ItemBase.h"

#include "EffectGrantingItem.generated.h"

class UGameplayEffect;
class UNiagaraSystem;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CustomValues|Effects")
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	UPROPERTY(EditDefaultsOnly, Category = "CustomValues|Visuals")
	UNiagaraSystem* OnCollectedEffect;

	UPROPERTY(EditDefaultsOnly, Category = "CustomValues|Idle")
	float RotationRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Idle")
	URotatingMovementComponent* RotationComponent;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void InteractRequest(AController* InteractingCharacter) override;

public:

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_OnCollected();

	TArray<TSubclassOf<UGameplayEffect>> GetEffects() { return Effects; }

};
