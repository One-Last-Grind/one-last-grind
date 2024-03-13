// Copyright Epic Games, Inc. All Rights Reserved.

#include "OneLastGrindGameMode.h"
#include "UObject/ConstructorHelpers.h"

AOneLastGrindGameMode::AOneLastGrindGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/OneLastGrind/Characters/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}


void AOneLastGrindGameMode::BeginPlay()
{
	Super::BeginPlay();

	//Bind our Player died delegate to the Gamemode's PlayerDied function.
	if (!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this, &AOneLastGrindGameMode::PlayerDied);
	}

}



void AOneLastGrindGameMode::RestartPlayer(AController* NewPlayer)
{
	if (LastCheckPoint != nullptr)
	{
		Super::RestartPlayerAtTransform(NewPlayer, LastCheckPoint->GetActorTransform());
	}
}

void AOneLastGrindGameMode::PlayerDied(AOneLastGrindPawn* Pawn)
{
	// Get a reference to our Character's player Controller
	AController* CharacterController = Pawn->GetController();
	RestartPlayer(CharacterController);
}
