// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/TitleHUD.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "UI/Widgets/SessionSelectorMenu.h"
#include "UI/Widgets/SessionButtonWidget.h"
#include "UI/Widgets/LoadingScreen.h"
#include "UI/Widgets/HostJoinMenu.h"
#include "UI/Widgets/TitleMenu.h"
#include "GameModes/TitleGameMode.h"


void ATitleHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ATitleHUD::BeginPlay IsValid(World) == false"));
		return;
	}

	ATitleGameMode* GameMode = Cast<ATitleGameMode>(World->GetAuthGameMode());
	if (IsValid(GameMode) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ATitleHUD::BeginPlay IsValid(GameMode) == false"));
		return;
	}

	if (PlayerOwner)
	{
		PlayerOwner->SetInputMode(FInputModeUIOnly());
		PlayerOwner->bShowMouseCursor = true;
	}

	TitleMenu = CreateWidget<UTitleMenu>(UGameplayStatics::GetGameInstance(World), TitleMenuClass);
	HostJoinMenu = CreateWidget<UHostJoinMenu>(UGameplayStatics::GetGameInstance(World), HostJoinMenuClass);
	LoadingScreen = CreateWidget<ULoadingScreen>(UGameplayStatics::GetGameInstance(World), LoadingScreenClass);
	SessionSelectorMenu = CreateWidget<USessionSelectorMenu>(UGameplayStatics::GetGameInstance(World), SessionSelectorMenuClass);

	TitleMenu->StartClicked.AddDynamic(this, &ATitleHUD::OpenHostJoinMenu);
	TitleMenu->ExitClicked.AddDynamic(GameMode, &ATitleGameMode::ExitGame);

	HostJoinMenu->HostClicked.AddDynamic(GameMode, &ATitleGameMode::HostSession);
	HostJoinMenu->JoinClicked.AddDynamic(this, &ATitleHUD::ShowSessionSelectorMenu);
	HostJoinMenu->BackClicked.AddDynamic(this, &ATitleHUD::OpenTitleMenu);

	SessionSelectorMenu->BackClicked.AddDynamic(this, &ATitleHUD::OpenHostJoinMenu);
	SessionSelectorMenu->RefreshClicked.AddDynamic(this, &ATitleHUD::ShowSessionSelectorMenu);

	GameMode->OnSessionsFound.AddDynamic(this, &ATitleHUD::FillSessions);

	OpenTitleMenu();
}

void ATitleHUD::OpenTitleMenu()
{
	ClearScreen();

	if (IsValid(TitleMenu) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ATitleHUD::OpenTitleMenu IsValid(TitleMenu) == false"));
		return;
	}

	TitleMenu->AddToViewport();
}

void ATitleHUD::OpenHostJoinMenu()
{
	ClearScreen();

	if (IsValid(HostJoinMenu) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ATitleHUD::OpenHostJoinMenu IsValid(HostJoinMenu) == false"));
		return;
	}

	HostJoinMenu->AddToViewport();
}

void ATitleHUD::ShowLoadingScreen(FText LoadingText)
{
	if (IsValid(LoadingScreen) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ATitleHUD::ShowLoadingScreen IsValid(LoadingScreen) == false"));
		return;
	}

	LoadingScreen->SetLoadingText(LoadingText);
	if(LoadingScreen->IsInViewport() == false) LoadingScreen->AddToViewport(10);
}

void ATitleHUD::RemoveLoadingScreen()
{
	if (IsValid(LoadingScreen) && LoadingScreen->IsInViewport())
	{
		LoadingScreen->RemoveFromParent();
	}
}

void ATitleHUD::ShowSessionSelectorMenu()
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ATitleHUD::ShowSessionSelectorMenu IsValid(World) == false"));
		return;
	}

	ATitleGameMode* GameMode = Cast<ATitleGameMode>(World->GetAuthGameMode());
	if (IsValid(GameMode) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ATitleHUD::ShowSessionSelectorMenu IsValid(GameMode) == false"));
		return;
	}
	if (IsValid(SessionSelectorMenu) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ATitleHUD::ShowSessionSelectorMenu IsValid(SessionSelectorMenu) == false"));
		return;
	}

	if (SessionSelectorMenu->IsInViewport() == false)
	{
		ClearScreen();
	}
	
	ShowLoadingScreen(FText::FromString("Looking for sessions..."));
	SessionSelectorMenu->ClearSessions();
	SessionSelectorMenu->AddToViewport();
	GameMode->FindSessions();
}

void ATitleHUD::FillSessions(TArray<FText> SessionNames)
{
	RemoveLoadingScreen();

	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ATitleHUD::FillSessions IsValid(World) == false"));
		return;
	}

	ATitleGameMode* GameMode = Cast<ATitleGameMode>(World->GetAuthGameMode());
	if (IsValid(GameMode) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ATitleHUD::FillSessions IsValid(GameMode) == false"));
		return;
	}

	USessionButtonWidget* SessionButton = nullptr;

	for (FText Session : SessionNames)
	{
		SessionButton = SessionSelectorMenu->AddSessionButton(Session);
		if (IsValid(SessionButton))
		{
			SessionButton->SessionClicked.AddDynamic(GameMode, &ATitleGameMode::JoinSession);
			SessionButton->SessionNameDelegate.AddDynamic(this, &ATitleHUD::ShowLoadingScreen);
		}
	}
}

void ATitleHUD::ClearScreen()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
}
