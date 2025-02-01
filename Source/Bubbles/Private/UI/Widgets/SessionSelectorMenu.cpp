// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/SessionSelectorMenu.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"

#include "UI/Widgets/SessionButtonWidget.h"


void USessionSelectorMenu::OnBackClicked()
{
	BackClicked.Broadcast();
}

void USessionSelectorMenu::NativeConstruct()
{
	Super::NativeConstruct();
	SessionScrollBox->ClearChildren();
}

void USessionSelectorMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BTN_Back->OnClicked.AddDynamic(this, &USessionSelectorMenu::OnBackClicked);

	SessionWidgetSwitcher->SetActiveWidgetIndex(0);
}

USessionButtonWidget* USessionSelectorMenu::AddSessionButton(FText SessionName)
{
	USessionButtonWidget* SessionButton = CreateWidget<USessionButtonWidget>(this, SessionButtonWidgetClass);
	if (IsValid(SessionButton) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("USessionSelectorMenu::AddSessionButton IsValid(SessionButton) == false"));
		return nullptr;
	}
	SessionButton->SetSessionName(SessionName);
	SessionScrollBox->AddChild(SessionButton);

	SessionWidgetSwitcher->SetActiveWidgetIndex(1);

	return SessionButton;
}

void USessionSelectorMenu::ClearSessions()
{
	SessionScrollBox->ClearChildren();
}
