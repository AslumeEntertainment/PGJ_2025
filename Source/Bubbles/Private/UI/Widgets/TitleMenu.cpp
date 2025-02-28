// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/TitleMenu.h"

#include "Components/Button.h"


void UTitleMenu::OnHostClicked()
{
	HostClicked.Broadcast();
}

void UTitleMenu::OnJoinClicked()
{
	JoinClicked.Broadcast();
}

void UTitleMenu::OnSettingsClicked()
{
	SettingsClicked.Broadcast();
}

void UTitleMenu::OnExitClicked()
{
	ExitClicked.Broadcast();
}

void UTitleMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BTN_Host->OnClicked.AddDynamic(this, &UTitleMenu::OnHostClicked);
	BTN_Join->OnClicked.AddDynamic(this, &UTitleMenu::OnJoinClicked);
	BTN_Settings->OnClicked.AddDynamic(this, &UTitleMenu::OnSettingsClicked);
	BTN_Exit->OnClicked.AddDynamic(this, &UTitleMenu::OnExitClicked);
}
