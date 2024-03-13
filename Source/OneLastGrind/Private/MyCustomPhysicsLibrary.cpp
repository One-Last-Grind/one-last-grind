#include "MyCustomPhysicsLibrary.h"

void UMyCustomPhysicsLibrary::SetPhysicalMaterialDetails(UStaticMeshComponent* Mesh, float StaticFriction, float DynamicFriction, bool bOverrideFrictionCombineMode, TEnumAsByte<EFrictionCombineMode::Type> FrictionCombineMode, float Restitution, bool bOverrideRestitutionCombineMode, TEnumAsByte<EFrictionCombineMode::Type> RestitutionCombineMode) {
	UPhysicalMaterial* PhysicalMaterial = NewObject<UPhysicalMaterial>();

	PhysicalMaterial->Friction = DynamicFriction;
	PhysicalMaterial->StaticFriction = StaticFriction;
	PhysicalMaterial->FrictionCombineMode = FrictionCombineMode;
	PhysicalMaterial->bOverrideFrictionCombineMode = bOverrideFrictionCombineMode;
	PhysicalMaterial->Restitution = Restitution;
	PhysicalMaterial->RestitutionCombineMode = RestitutionCombineMode;
	PhysicalMaterial->bOverrideRestitutionCombineMode = bOverrideRestitutionCombineMode;

	PhysicalMaterial->AddToRoot(); // this is done to prevent it from being garbage collected
	Mesh->SetPhysMaterialOverride(PhysicalMaterial);
}