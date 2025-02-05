// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/GameOverWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UGameOverWidget::OnPlayAgainClicked()
{
	PlayAgainClicked.Broadcast();
}

void UGameOverWidget::OnQuitClicked()
{
	QuitClicked.Broadcast();
}

void UGameOverWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BTN_PlayAgain->OnClicked.AddDynamic(this, &UGameOverWidget::OnPlayAgainClicked);
	BTN_Quit->OnClicked.AddDynamic(this, &UGameOverWidget::OnQuitClicked);
}

void UGameOverWidget::SetGameOverMessage(FText GameOverMessage)
{
	TXT_GameOverMessage->SetText(GameOverMessage);
}
