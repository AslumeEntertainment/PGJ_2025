// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/InteractionWidget.h"

#include "Components/TextBlock.h"

void UInteractionWidget::SetInteractionText(FText Text)
{
	InteractionText->SetText(Text);
}
