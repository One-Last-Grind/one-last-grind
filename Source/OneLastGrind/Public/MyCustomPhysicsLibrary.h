// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "MyCustomPhysicsLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ONELASTGRIND_API UMyCustomPhysicsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "MyCustomPhysicsLibrary", Meta = (DefaultToSelf = Object, DisplayName = "Set Physical Material Details", Keywords = " MyCustomPhysicsLibrary physical physics material friction restitution"))
	static void SetPhysicalMaterialDetails(UStaticMeshComponent* Mesh, float StaticFriction, float DynamicFriction, bool bOverrideFrictionCombineMode, TEnumAsByte<EFrictionCombineMode::Type> FrictionCombineMode, float Restitution, bool bOverrideRestitutionCombineMode, TEnumAsByte<EFrictionCombineMode::Type> RestitutionCombineMode);
};