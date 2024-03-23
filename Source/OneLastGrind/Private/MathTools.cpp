// Fill out your copyright notice in the Description page of Project Settings.


#include "MathTools.h"

// Sets default values
AMathTools::AMathTools()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMathTools::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMathTools::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMathTools::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FVector AMathTools::SkateNormals(TArray<FVector> HitPointList, const FVector& SkateUpVector)
{
	FVector BestNormal;
	double DotWithBestNormal = -1;
	for (size_t i = 0; i < HitPointList.Num(); i++)
	{
		TArray<FVector> CpHitPointList = HitPointList;
		CpHitPointList.RemoveAt(i);

		FVector Diff1 = CpHitPointList[0] - CpHitPointList[1];
		FVector Diff2 = CpHitPointList[1] - CpHitPointList[2];

		FVector Normal = Diff1.Cross(Diff2).GetSafeNormal();
		double DotProduct = SkateUpVector.Dot(Normal);

		if (DotProduct > DotWithBestNormal)
		{
			DotWithBestNormal = DotProduct;
			BestNormal = Normal;
		}		
	}
	return BestNormal;
}