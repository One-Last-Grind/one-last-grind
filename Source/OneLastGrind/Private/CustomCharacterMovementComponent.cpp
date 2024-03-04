// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacterMovementComponent.h"
#include "OneLastGrindCharacter.h"
#include "Engine/Engine.h"

// Initializations
UCustomCharacterMovementComponent::UCustomCharacterMovementComponent()
{
}

void UCustomCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OLGCharacterOwner = Cast<AOneLastGrindCharacter>(GetOwner());

	SetMovementMode(MOVE_Custom, CMOVE_Skate);
}

bool UCustomCharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode)
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}




void UCustomCharacterMovementComponent::PhysWalking(float deltaTime, int32 Iterations)
{
	//Super::PhysWalking(deltaTime, Iterations);

	FString DebugString = FString::Printf(TEXT("Current Movement Mode: %s"), *GetMovementName());
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::White, DebugString, false, FVector2D::UnitVector * 1.2f);
	FHitResult SurfaceHit;



	if (!CheckFloor(SurfaceHit)) {

		SetMovementMode(MOVE_Falling);
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	// Ensure velocity is horizontal.
	MaintainHorizontalGroundVelocity();

	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, false);

	FVector SlopeForce = CurrentFloor.HitResult.Normal;
	SlopeForce.Z = 0.f;
	Velocity += SlopeForce * SkateGravityForce * deltaTime;

	
	
	FStepDownResult StepDownResult;
	MoveAlongFloor(Velocity, deltaTime, &StepDownResult);


	if (StepDownResult.bComputedFloor && StepDownResult.FloorResult.bWalkableFloor)
	{
		Velocity = FVector::ZeroVector;
	}

	
	FString DebugTest = FString::Printf(TEXT("Find floor: %s"), *CurrentFloor.HitResult.Normal.ToString());
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::White, DebugTest, false, FVector2D::UnitVector * 1.2f);


	//FHitResult Hit(1.f);
	//SafeMoveUpdatedComponent(Velocity, OLGCharacterOwner->GetActorQuat(), true, Hit);
}

// Custom Phys Functions
void UCustomCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case CMOVE_Skate:
		PhysSkate(deltaTime, Iterations);
		break;
	default:
		FString DebugString = FString::Printf(TEXT("AYO CUSTOM "));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::White, DebugString, false, FVector2D::UnitVector * 1.2f);
		break;
	}
}


void UCustomCharacterMovementComponent::PhysSkate(float deltaTime, int32 Iterations)
{
	FString DebugString = FString::Printf(TEXT("Current Movement Mode: %s"), *GetMovementName());
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::White, DebugString, false, FVector2D::UnitVector * 1.2f);
	FHitResult SurfaceHit;


	if (!CheckFloor(SurfaceHit)) {

		SetMovementMode(MOVE_Falling);
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	/*FVector Gravity = FVector::DownVector * SkateGravityForce;
	FVector PerpGravity = -SurfaceHit.ImpactNormal.GetSafeNormal() * SkateGravityForce;
	FVector ParGravity = Gravity - PerpGravity;

	Velocity += ParGravity * deltaTime;

	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Velocity, UpdatedComponent->GetComponentQuat(), true, Hit);*/

	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, false);
	// Ensure velocity is horizontal.
	MaintainHorizontalGroundVelocity();

	FVector SlopeForce = CurrentFloor.HitResult.Normal;
	SlopeForce.Z = 0.f;
	DrawDebugDirectionalArrow(GetWorld(), UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + SlopeForce * 100, 1.0f, FColor::Green, false, 0, 3 * DEBUG_ARROW_THICNKESS);


	Velocity += SlopeForce * SkateGravityForce * deltaTime;

	Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector().GetSafeNormal2D());

	// Apply acceleration
	CalcVelocity(deltaTime, GroundFriction * SkateFrictionFactor, false, GetMaxBrakingDeceleration());

	// try to move forward
	MoveAlongFloor(Velocity, deltaTime);

	SafeMoveUpdatedComponent(Velocity, UpdatedComponent->GetComponentQuat(), true, SurfaceHit);
}




bool UCustomCharacterMovementComponent::CheckFloor(FHitResult& Hit) const
{
	if (OLGCharacterOwner)
	{
		// Get the floor surface
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OLGCharacterOwner);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		FVector Start = UpdatedComponent->GetComponentLocation();
		FVector End = Start + OLGCharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f * FVector::DownVector;

		FName ProfileName = TEXT("BlockAll");

		bool bHit = GetWorld()->LineTraceSingleByProfile(Hit, Start, End, ProfileName, QueryParams);

		if (bHit)
		{
			// Calculate end point of the normal line
			FVector NormalEnd = Hit.ImpactPoint + Hit.ImpactNormal * 50.0f; // Adjust length of the normal line as needed

			// Calc gravity
			FVector Gravity = FVector::DownVector * SkateGravityForce;
			DrawDebugDirectionalArrow(GetWorld(), Start, Start + Gravity, 1.0f, FColor::Yellow, false, 0, DEBUG_ARROW_THICNKESS);

			// perpendicular gravity
			FVector PerpGravity = - Hit.ImpactNormal.GetSafeNormal() * SkateGravityForce;
			DrawDebugDirectionalArrow(GetWorld(), Start, Start + PerpGravity, 1.0f, FColor::Red, false, 0, DEBUG_ARROW_THICNKESS);
			
			// paralel gravity
			FVector ParGravity = Gravity - PerpGravity;
			DrawDebugDirectionalArrow(GetWorld(), Start + PerpGravity, Start + PerpGravity + ParGravity, 1.0f, FColor::Cyan, false, 0, DEBUG_ARROW_THICNKESS);
		}

		return bHit;
	}

	return false;
}

// Tick
void UCustomCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugString = FString::Printf(TEXT("Current Movement Mode: %s"), *GetMovementName());
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::White, DebugString, false, FVector2D::UnitVector * 1.2f);
}


