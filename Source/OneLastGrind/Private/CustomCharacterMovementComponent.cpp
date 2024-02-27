// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacterMovementComponent.h"
#include "Engine/Engine.h"

UCustomCharacterMovementComponent::UCustomCharacterMovementComponent()
{
}

void UCustomCharacterMovementComponent::PhysWalking(float deltaTime, int32 Iterations)
{
	Super::PhysWalking(deltaTime, Iterations);

	FString MyDebugString = FString::Printf(TEXT("custom walking called!"));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.1f, FColor::Yellow, MyDebugString, false, FVector2D::UnitVector * 1.2f);


}

void UCustomCharacterMovementComponent::PhysFalling(float deltaTime, int32 Iterations)
{
	Super::PhysFalling(deltaTime, Iterations);

	FString MyDebugString = FString::Printf(TEXT("custom falling called!"));
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.1f, FColor::Yellow, MyDebugString, false, FVector2D::UnitVector * 1.2f);
}
