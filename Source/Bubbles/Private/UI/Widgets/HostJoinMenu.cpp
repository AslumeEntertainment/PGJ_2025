// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/HostJoinMenu.h"

#include "Components/Button.h"

void UHostJoinMenu::OnHostClicked()
{
	HostClicked.Broadcast();
}

void UHostJoinMenu::OnJoinClicked()
{
	JoinClicked.Broadcast();
}

void UHostJoinMenu::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UHostJoinMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BTN_Host->OnClicked.AddDynamic(this, &UHostJoinMenu::OnHostClicked);
	BTN_Join->OnClicked.AddDynamic(this, &UHostJoinMenu::OnJoinClicked);
	BTN_Back->OnClicked.AddDynamic(this, &UHostJoinMenu::OnBackClicked);
}
