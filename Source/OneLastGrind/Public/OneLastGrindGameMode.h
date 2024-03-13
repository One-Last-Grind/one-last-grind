// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OneLastGrindPawn.h"
#include "OneLastGrindGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedSignature, AOneLastGrindPawn*, Character);

UCLASS(minimalapi)
class AOneLastGrindGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOneLastGrindGameMode();

	UPROPERTY(BlueprintReadWrite)
	AActor* LastCheckPoint;

	const FOnPlayerDiedSignature& GetOnPlayerDied() const { return OnPlayerDied; }

	// Tries to spawn the player's pawn
	virtual void RestartPlayer(AController* NewPlayer) override;


protected:
	virtual void BeginPlay() override;

	// Called when Player pawn dies
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	virtual void PlayerDied(AOneLastGrindPawn* Pawn);

	// Signature to bind delegate
	UPROPERTY()
	FOnPlayerDiedSignature OnPlayerDied;

};



