// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TitleHUD.generated.h"

class UTitleMenu;
class UHostJoinMenu;
class ULoadingScreen;
class USessionCreatorMenu;
class USessionSelectorMenu;

UCLASS()
class BUBBLES_API ATitleHUD : public AHUD
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClasses");
	TSubclassOf<UTitleMenu> TitleMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClasses");
	TSubclassOf<UHostJoinMenu> HostJoinMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClasses");
	TSubclassOf<ULoadingScreen> LoadingScreenClass;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClasses");
	TSubclassOf<USessionCreatorMenu> SessionCreatorMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetClasses");
	TSubclassOf<USessionSelectorMenu> SessionSelectorMenuClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTitleMenu* TitleMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UHostJoinMenu* HostJoinMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ULoadingScreen* LoadingScreen;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USessionCreatorMenu* SessionCreatorMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USessionSelectorMenu* SessionSelectorMenu;

public:

	UFUNCTION()
	void OpenTitleMenu();

	UFUNCTION()
	void OpenHostJoinMenu();

	UFUNCTION()
	void ShowLoadingScreen(FText LoadingText = FText::FromString("Loading..."));

	UFUNCTION()
	void RemoveLoadingScreen();

	UFUNCTION()
	void OpenSessionCreatorMenu();

	UFUNCTION()
	void ShowSessionSelectorMenu();

	UFUNCTION()
	void FillSessions(TArray<FText> SessionNames);

	void ClearScreen();

};
