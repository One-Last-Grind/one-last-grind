// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "LevelIndicator.h"
#include "OneLastGrindFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ONELASTGRIND_API UOneLastGrindFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UOneLastGrindFunctionLibrary", Meta = (DefaultToSelf = Object, DisplayName = "Set Physical Material Details", Keywords = "MyCustomPhysicsLibrary physical physics material friction restitution"))
	static void SetPhysicalMaterialDetails(UStaticMeshComponent* Mesh, float StaticFriction, float DynamicFriction, bool bOverrideFrictionCombineMode, TEnumAsByte<EFrictionCombineMode::Type> FrictionCombineMode, float Restitution, bool bOverrideRestitutionCombineMode, TEnumAsByte<EFrictionCombineMode::Type> RestitutionCombineMode);
	
	UFUNCTION(BlueprintCallable, Category = "UOneLastGrindFunctionLibrary", Meta = (DefaultToSelf = Object, DisplayName = "Add Level Offset", Keywords = "MyCustomPhysicsLibrary"))
	static void SetLevelOffset(UObject* WorldObject, TSoftObjectPtr<UWorld> Level, FVector WorldOffset, bool WorldShift);

	UFUNCTION(BlueprintCallable, Category = "UOneLastGrindFunctionLibrary", Meta = (DefaultToSelf = Object, DisplayName = "Get Level Offset", Keywords = "MyCustomPhysicsLibrary"))
	static void GetLevelOffset(UObject* WorldObject, TSoftObjectPtr<UWorld> CurrentLevel, TSoftObjectPtr<UWorld> NextLevel, ALevelIndicator*& CurrentLevelIndicator, ALevelIndicator*& NextLevelIndicator);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UOneLastGrindFunctionLibrary", Meta = (DefaultToSelf = Object, DisplayName = "Get Level Name", Keywords = "MyCustomPhysicsLibrary"))
	static const FName GetLevelName(TSoftObjectPtr<UWorld> Level);
};
