// Fill out your copyright notice in the Description page of Project Settings.


#include "OneLastGrindPawn.h"
#include "OneLastGrindGameMode.h"

// Sets default values
AOneLastGrindPawn::AOneLastGrindPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOneLastGrindPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called when actor is destroyed
void AOneLastGrindPawn::Destroyed()
{
	Super::Destroyed();

	if (UWorld* World = GetWorld())
	{
		if (AOneLastGrindGameMode* GameMode = Cast<AOneLastGrindGameMode>(World->GetAuthGameMode()))
		{
			GameMode->GetOnPlayerDied().Broadcast(this);
		}
	}
}
void AOneLastGrindPawn::CallRespawnPlayer()
{
	AController* ControllerRef = GetController();


	// Destroy the player
	Destroy();

	if (UWorld* World = GetWorld())
	{
		if (AOneLastGrindGameMode* GameMode = Cast<AOneLastGrindGameMode>(World->GetAuthGameMode()))
		{
			GameMode->RestartPlayer(ControllerRef);
		}
	}
}
// Called every frame
void AOneLastGrindPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void AOneLastGrindPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

