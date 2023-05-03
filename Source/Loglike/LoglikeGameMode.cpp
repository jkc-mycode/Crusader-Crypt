// Copyright Epic Games, Inc. All Rights Reserved.

#include "LoglikeGameMode.h"
#include "Character/LoglikeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALoglikeGameMode::ALoglikeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
