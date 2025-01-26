// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/InGameHUD.h"

#include "Kismet/GameplayStatics.h"

#include "Characters/BubbleCharacter.h"
#include "UI/InteractionWidget.h"


void AInGameHUD::BeginPlay()
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		return;
	}

	if (PlayerOwner)
	{
		PlayerOwner->SetInputMode(FInputModeGameOnly());
		PlayerOwner->bShowMouseCursor = false;
	}

	/*InteractionWidget = CreateWidget<UInteractionWidget>(UGameplayStatics::GetGameInstance(World), InteractionWidgetClass);
	ABubbleCharacter* BubbleCharacter = Cast<ABubbleCharacter>(PlayerOwner->GetPawn());
	if (IsValid(BubbleCharacter))
	{
		BubbleCharacter->InteractIndicationTextDelegate.AddDynamic(InteractionWidget, &UInteractionWidget::SetInteractionText);
	}


	ToggleInteractionWidget(true);*/

}

void AInGameHUD::ToggleInteractionWidget(bool bShouldShow)
{
	if (IsValid(InteractionWidget) == false || IsValid(PlayerOwner) == false)
	{
		return;
	}
	
	if ((bShouldShow && InteractionWidget->IsInViewport()) || !bShouldShow && !InteractionWidget->IsInViewport())
	{
		return;
	}
	
	if (bShouldShow)
	{
		InteractionWidget->AddToViewport();
	}
	else
	{
		InteractionWidget->RemoveFromParent();
	}
}
