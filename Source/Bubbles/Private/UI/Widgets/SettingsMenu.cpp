// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/SettingsMenu.h"

#include "Components/Button.h"
#include "Components/Slider.h"


void USettingsMenu::OnBackClicked()
{
	BackClicked.Broadcast();
}

void USettingsMenu::OnLowClicked()
{
	GraphicsUpdated.Broadcast(0);
	SetCurrentGraphicsQualitySelected(0);
}

void USettingsMenu::OnMediumClicked()
{
	GraphicsUpdated.Broadcast(1);
	SetCurrentGraphicsQualitySelected(1);
}

void USettingsMenu::OnHighClicked()
{
	GraphicsUpdated.Broadcast(2);
	SetCurrentGraphicsQualitySelected(2);
}

void USettingsMenu::OnEpicClicked()
{
	GraphicsUpdated.Broadcast(3);
	SetCurrentGraphicsQualitySelected(3);
}

void USettingsMenu::OnCinematicClicked()
{
	GraphicsUpdated.Broadcast(4);
	SetCurrentGraphicsQualitySelected(4);
}

void USettingsMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BTN_Back->OnClicked.AddDynamic(this, &USettingsMenu::OnBackClicked);
	BTN_Low->OnClicked.AddDynamic(this, &USettingsMenu::OnLowClicked);
	BTN_Medium->OnClicked.AddDynamic(this, &USettingsMenu::OnMediumClicked);
	BTN_High->OnClicked.AddDynamic(this, &USettingsMenu::OnHighClicked);
	BTN_Epic->OnClicked.AddDynamic(this, &USettingsMenu::OnEpicClicked);
	BTN_Cinematic->OnClicked.AddDynamic(this, &USettingsMenu::OnCinematicClicked);
}

void USettingsMenu::SetCurrentGraphicsQualitySelected(int CurrentScalability)
{
	TArray<UButton*> ScalabilityButtons;
	ScalabilityButtons.Add(BTN_Low);
	ScalabilityButtons.Add(BTN_Medium);
	ScalabilityButtons.Add(BTN_High);
	ScalabilityButtons.Add(BTN_Epic);
	ScalabilityButtons.Add(BTN_Cinematic);

	for (int i = 0; i < 5; i++)
	{
		ScalabilityButtons[i]->SetIsEnabled(i == CurrentScalability ? false : true);
	}
}


