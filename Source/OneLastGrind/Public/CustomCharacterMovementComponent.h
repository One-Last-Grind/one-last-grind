// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OneLastGrind.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None		UMETA(Hidden),
	CMOVE_Skate		UMETA(DisplayName = "Skate"),
	CMOVE_MAX		UMETA(Hidden),
};


/**
 * 
 */
UCLASS()
class ONELASTGRIND_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()


	// Transient
	UPROPERTY(Transient) AOneLastGrindCharacter* OneLastGrindCharacterOwner;
public:
	UCustomCharacterMovementComponent();

	UFUNCTION(BlueprintPure) bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode);
	virtual void PhysWalking(float deltaTime, int32 Iterations) override;
	virtual void PhysFalling(float deltaTime, int32 Iterations) override;


protected:
	virtual void InitializeComponent() override;

};

