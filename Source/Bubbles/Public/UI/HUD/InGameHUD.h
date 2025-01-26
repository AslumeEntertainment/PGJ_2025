// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

class UInteractionWidget;

UCLASS()
class BUBBLES_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClasses");
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInteractionWidget* InteractionWidget;

public:

	void ToggleInteractionWidget(bool bShouldShow);
};
