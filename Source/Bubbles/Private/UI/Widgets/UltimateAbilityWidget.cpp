// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/UltimateAbilityWidget.h"

#include "Components/TextBlock.h"

void UUltimateAbilityWidget::SetUltimateAbilityText(FText TopText, FText BottomText, FLinearColor TextColor)
{
	TXT_TopText->SetText(TopText);
	TXT_TopText->SetColorAndOpacity(TextColor);

	TXT_BottomText->SetText(BottomText);
	TXT_BottomText->SetColorAndOpacity(TextColor);
}
