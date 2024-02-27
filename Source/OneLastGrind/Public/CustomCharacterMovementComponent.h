// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ONELASTGRIND_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()


public:
	UCustomCharacterMovementComponent();

	virtual void PhysWalking(float deltaTime, int32 Iterations) override;
	virtual void PhysFalling(float deltaTime, int32 Iterations) override;

};

