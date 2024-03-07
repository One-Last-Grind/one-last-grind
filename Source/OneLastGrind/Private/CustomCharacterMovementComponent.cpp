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
	//MaintainHorizontalGroundVelocity();

	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, false);

	FVector SlopeVector = CurrentFloor.HitResult.Normal;
	SlopeVector.Z = 0.f;
	Velocity += SlopeVector * SkateGravityForce * deltaTime;

	
	
	//FStepDownResult StepDownResult;
	//MoveAlongFloor(Velocity, deltaTime, &StepDownResult);

	const FVector Delta = RotateGravityToWorld(RotateWorldToGravity(Velocity) * FVector(1.0, 1.0, 0.0)) * deltaTime;

	FVector RampVector = ComputeGroundMovementDelta(Delta, CurrentFloor.HitResult, CurrentFloor.bLineTrace);

	DrawDebugDirectionalArrow(GetWorld(), UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + RampVector * 100.0f, 1.0f, FColor::Cyan, false, 0, DEBUG_ARROW_THICNKESS);
	DrawDebugDirectionalArrow(GetWorld(), UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + Delta * 100.f, 1.0f, FColor::Purple, false, 0, DEBUG_ARROW_THICNKESS);


	float DesiredPitch = 40.f; // Assign your desired pitch angle here

	// Convert the pitch angle to radians
	float PitchRadians = FMath::DegreesToRadians(DesiredPitch);

	// Create a rotation quaternion with only pitch rotation
	FQuat PitchRotation = FQuat(FVector::ZAxisVector, 45.f);


	//Rotate player to slope if grounded
	FQuat NewRotation = UpdatedComponent->GetComponentRotation().Quaternion();
	FVector NormalAverage;

	
	if (GetSkateSurfaceNormalAvg(NormalAverage))
	{
		DrawDebugDirectionalArrow(GetWorld(), UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + NormalAverage * 100.f, 1.0f, FColor::Emerald, false, 0, DEBUG_ARROW_THICNKESS);
		// Z axis
		FRotator GroundAlignment = FRotationMatrix::MakeFromZX(NormalAverage, UpdatedComponent->GetForwardVector()).Rotator();
		
		//GroundAlignment = FRotator(0.0f, 0.f, 45.f).Quaternion();
		// Interpolate rotation smoothly
		NewRotation = FMath::RInterpTo(UpdatedComponent->GetComponentRotation(), GroundAlignment, deltaTime, 15.f).Quaternion();

		//NewRotation = FQuat::Slerp(UpdatedComponent->GetComponentRotation().Quaternion(), GroundAlignment, deltaTime * 15.0f);
	}
	


	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(RampVector, NewRotation, true, Hit);

	FString DebugRotation = FString::Printf(TEXT("New rotation: %s"), *UpdatedComponent->GetComponentQuat().ToString());
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::White, DebugRotation, false, FVector2D::UnitVector * 1.2f);

	//if (StepDownResult.bComputedFloor && StepDownResult.FloorResult.bWalkableFloor)
	//{
	//	Velocity = FVector::ZeroVector;
	//}

	
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
	FString DebugString = FString::Printf(TEXT("Currently Skating"));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::White, DebugString, false, FVector2D::UnitVector * 1.2f);
	FHitResult SurfaceHit;

	if (!CheckFloor(SurfaceHit)) {

		SetMovementMode(MOVE_Falling);
		StartNewPhysics(deltaTime, Iterations);
		return;
	}


	FVector Gravity = FVector::DownVector;
	FVector ParGravity = FVector::DotProduct(Gravity, SurfaceHit.Normal) * SurfaceHit.Normal;
	FVector PerpGravity = Gravity - ParGravity;
	
	float ParAcceleration = ParGravity.Size() / Mass;
	float SlidingTime = 10.f;
	float SlidingVelocity = ParAcceleration * SlidingTime;

	Velocity += SkateGravityForce * SurfaceHit.Normal * SlidingVelocity * deltaTime;
	Velocity += SkateGravityForce * FVector::DownVector * deltaTime;


	//Turn to local coordinates because we are rotating the skater
	double localForwardVelocity = FVector::DotProduct(UpdatedComponent->GetForwardVector(), Velocity);
	double localUpVelocity = FVector::DotProduct(UpdatedComponent->GetUpVector(), Velocity);
	double localRightVelocity = FVector::DotProduct(UpdatedComponent->GetRightVector(), Velocity);

	//constrain to only moving forward/backward (with a little bit of sliding based on SidewaysWheelSlide value)
	localRightVelocity *= 0.5f;

	//convert to global for velocity
	Velocity = localForwardVelocity * UpdatedComponent->GetForwardVector() + localUpVelocity * UpdatedComponent->GetUpVector() + localRightVelocity * UpdatedComponent->GetRightVector();


	FVector Adjusted = Velocity * deltaTime;

	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);
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

bool UCustomCharacterMovementComponent::GetSkateSurfaceNormalAvg(FVector& normalAverage) const
{
	UCapsuleComponent* CharacterCapsule = CharacterOwner->GetCapsuleComponent(); // Replace with your character's capsule component reference
	FVector ForwardVector = CharacterOwner->GetActorForwardVector();
	FVector DownVector = CharacterOwner->GetActorUpVector();

	// Calculate the start and end locations for the first line trace (front of the capsule)
	FVector StartLocationFront = CharacterCapsule->GetComponentLocation() + (ForwardVector * CharacterCapsule->GetScaledCapsuleRadius());
	FVector EndLocationFront = StartLocationFront - (DownVector * CharacterCapsule->GetScaledCapsuleHalfHeight() * 1.5f);
	FHitResult FirstHitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("Trace")), true, nullptr);

	DrawDebugLine(GetWorld(), StartLocationFront, EndLocationFront, FColor::Purple, false, .1f, 0, 5);
	bool isFrontGrounded = GetWorld()->LineTraceSingleByChannel(FirstHitResult, StartLocationFront, EndLocationFront, ECC_Visibility, TraceParams);

	// Calculate the start and end locations for the second line trace (back of the capsule)
	FVector StartLocationBack = CharacterCapsule->GetComponentLocation() - (ForwardVector * CharacterCapsule->GetScaledCapsuleRadius());
	FVector EndLocationBack = StartLocationBack - (DownVector * CharacterCapsule->GetScaledCapsuleHalfHeight() * 1.5f);
	FHitResult SecondHitResult;
	DrawDebugLine(GetWorld(), StartLocationBack, EndLocationBack, FColor::Purple, false, .1f, 0, 5);
	bool isBackGrounded = GetWorld()->LineTraceSingleByChannel(SecondHitResult, StartLocationBack, EndLocationBack, ECC_Visibility, TraceParams);

	// Calculate the average normal vector
	FVector AverageNormal = (FirstHitResult.Normal + SecondHitResult.Normal).GetSafeNormal();

	if (!isBackGrounded && !isFrontGrounded)
	{
		return false;
	}

	normalAverage = AverageNormal;
	return true;
}

// Tick
void UCustomCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugString = FString::Printf(TEXT("Current Movement Mode: %s"), *GetMovementName());
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::White, DebugString, false, FVector2D::UnitVector * 1.2f);
}


