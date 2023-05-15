// Copyright Epic Games, Inc. All Rights Reserved.

#include "LoglikeGameMode.h"
#include "Character/LoglikeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ABGameState.h"
#include "Character/ABPlayerState.h"

ALoglikeGameMode::ALoglikeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ALoglikeGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeGameOnly());
}

void ALoglikeGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABGameState = Cast<AABGameState>(GameState);
}

void ALoglikeGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto ABPlayerState = Cast<AABPlayerState>(NewPlayer->PlayerState);
	ABPlayerState->InitPlayerData();
}
