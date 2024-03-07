// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OneLastGrind.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h" 
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
	UPROPERTY(Transient) AOneLastGrindCharacter* OLGCharacterOwner;


	// Parameters
	// 
	// Slide
	UPROPERTY(EditDefaultsOnly) float MinSkateSpeed = 400.f;
	UPROPERTY(EditDefaultsOnly) float MaxSkateSpeed = 400.f;
	UPROPERTY(EditDefaultsOnly) float SkateEnterImpulse = 400.f;
	UPROPERTY(EditDefaultsOnly) float SkateGravityForce = 4000.f;
	UPROPERTY(EditDefaultsOnly) float SkateFrictionFactor = .06f;
	UPROPERTY(EditDefaultsOnly) float BrakingDecelerationSkating = 1000.f;
	UPROPERTY(EditDefaultsOnly) float SkateRotateAlignementSpeed = 15.f;


	

public:
	UCustomCharacterMovementComponent();

	UFUNCTION(BlueprintPure) bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode);

	

// Custom Movement Mode
private:
	float DEBUG_ARROW_THICNKESS = 2.f;


	void PhysSkate(float deltaTime, int32 Iterations);


	// Helper functions
	bool CheckFloor(FHitResult& Hit) const;
	bool GetSkateSurfaceNormalAvg(FVector& normalAverage) const;




protected:
// Overriden Functions
	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PhysWalking(float deltaTime, int32 Iterations) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
};

