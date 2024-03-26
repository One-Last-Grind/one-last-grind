// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "MySplineActor.generated.h"

class UMySplineMetadata;
class UMySplineComponent;

UCLASS()
class ONELASTGRIND_API AMySplineActor : public APawn
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void GetPropertyValueAtSplinePoint(int index, int& IsStopPoint, int& SplineDirection);

	AMySplineActor();
	UMySplineMetadata* GetSplineMetadata() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMySplineComponent* MySplineComponent = nullptr;

private:
	UPROPERTY(Instanced, Export)
	UMySplineMetadata* MySplineMetadata = nullptr;

};