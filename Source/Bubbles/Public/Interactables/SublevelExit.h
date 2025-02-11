// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/ItemBase.h"
#include "SublevelExit.generated.h"


UCLASS()
class BUBBLES_API ASublevelExit : public AItemBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "SublevelExitProperties")
	bool bIsSafeExit = false;

public:

	virtual void InteractRequest(AController* InteractingCharacter) override;

	virtual bool bCanInteract(AController* InteractingCharacter) override;

};
