// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/TitleMenu.h"

#include "Components/Button.h"


void UTitleMenu::OnStartClicked()
{
	StartClicked.Broadcast();
}

void UTitleMenu::OnExitClicked()
{
	ExitClicked.Broadcast();
}

void UTitleMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BTN_Start->OnClicked.AddDynamic(this, &UTitleMenu::OnStartClicked);
	BTN_Exit->OnClicked.AddDynamic(this, &UTitleMenu::OnExitClicked);
}
