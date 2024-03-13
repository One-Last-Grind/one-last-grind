// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OneLastGrindPawn.generated.h"

UCLASS()
class ONELASTGRIND_API AOneLastGrindPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AOneLastGrindPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when actor is destroyed during Gameplay
	virtual void Destroyed();

	UFUNCTION(BlueprintCallable, Category = "Respawn")
	// Call GameMode class to respawn player pawn
	void CallRespawnPlayer();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
