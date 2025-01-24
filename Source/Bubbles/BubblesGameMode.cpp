// Copyright Epic Games, Inc. All Rights Reserved.

#include "BubblesGameMode.h"
#include "BubblesCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABubblesGameMode::ABubblesGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
