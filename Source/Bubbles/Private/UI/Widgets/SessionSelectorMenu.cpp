// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/SessionSelectorMenu.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"

#include "UI/Widgets/SessionButtonWidget.h"


void USessionSelectorMenu::OnBackClicked()
{
	BackClicked.Broadcast();
}

void USessionSelectorMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BTN_Back->OnClicked.AddDynamic(this, &USessionSelectorMenu::OnBackClicked);
}

void USessionSelectorMenu::AddSessionButton(USessionButtonWidget* SessionButton)
{
	SessionScrollBox->AddChild(SessionButton);
}

void USessionSelectorMenu::ClearSessions()
{
	SessionScrollBox->ClearChildren();
}
