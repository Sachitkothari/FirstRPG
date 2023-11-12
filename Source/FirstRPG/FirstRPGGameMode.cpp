// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstRPGGameMode.h"
#include "FirstRPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFirstRPGGameMode::AFirstRPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_AssassinCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
