// Copyright Epic Games, Inc. All Rights Reserved.

#include "OneLastGrindGameMode.h"
#include "OneLastGrindCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOneLastGrindGameMode::AOneLastGrindGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
