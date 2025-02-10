// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/SessionButtonWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void USessionButtonWidget::OnSessionClicked()
{
	if (IsValid(GetParent()) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("USessionButtonWidget::OnSessionClicked IsValid(GetParent()) == false"));
		return;
	}

	SessionClicked.Broadcast(GetParent()->GetChildIndex(this));
	SessionNameDelegate.Broadcast(FText::FromString("Joining " + TXT_SessionName->GetText().ToString()));
	BTN_Session->SetIsEnabled(false);
}

void USessionButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BTN_Session->OnClicked.AddDynamic(this, &USessionButtonWidget::OnSessionClicked);
}

void USessionButtonWidget::SetSessionName(FText SessionName)
{
	TXT_SessionName->SetText(SessionName);
}
