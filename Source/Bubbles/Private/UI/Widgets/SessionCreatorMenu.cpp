// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/SessionCreatorMenu.h"

#include "Components/EditableText.h"
#include "Components/CheckBox.h"
#include "Components/Button.h"


void USessionCreatorMenu::OnStartClicked()
{
	if (ETXT_SessionName->GetText().IsEmpty())
	{
		return;
	}
	const FString& NameRef = ETXT_SessionName->GetText().ToString();
	StartClicked.Broadcast(NameRef, CB_IsPublic->IsChecked());
}

void USessionCreatorMenu::OnBackClicked()
{
	BackClicked.Broadcast();
}

void USessionCreatorMenu::NativeConstruct()
{
	Super::NativeConstruct();

	FString SessionName = "Session#" + FString::FromInt(FMath::RandRange(10000, 99999));
	ETXT_SessionName->SetText(FText::FromString(SessionName));
}

void USessionCreatorMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	BTN_Start->OnClicked.AddDynamic(this, &USessionCreatorMenu::OnStartClicked);
	BTN_Back->OnClicked.AddDynamic(this, &USessionCreatorMenu::OnBackClicked);
}
