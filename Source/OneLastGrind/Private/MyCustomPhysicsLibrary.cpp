#include "MyCustomPhysicsLibrary.h"
#include "Kismet/GameplayStatics.h"

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

void UMyCustomPhysicsLibrary::SetLevelOffset(UObject* WorldObject, FName Level, FVector WorldOffset, bool WorldShift) {
	UObject* WorldContextObject = WorldObject->GetWorld();
	ULevelStreaming* LevelStreaming = UGameplayStatics::GetStreamingLevel(WorldContextObject, Level);
	if (!LevelStreaming) return;
	ULevel* StreamedLevel = LevelStreaming->GetLoadedLevel();

	if (!StreamedLevel)
		return;
	StreamedLevel->ApplyWorldOffset(WorldOffset, WorldShift);

	if (!WorldShift)
	{
		UWorld* World = WorldContextObject->GetWorld();
		World->UpdateLevelStreaming();
	}

}

ALevelIndicator* UMyCustomPhysicsLibrary::GetLevelOffset(UObject* WorldObject, FName Level)
{
	UObject* WorldContextObject = WorldObject->GetWorld();
	ULevelStreaming* LevelStreaming = UGameplayStatics::GetStreamingLevel(WorldContextObject, Level);
	if (!LevelStreaming) return nullptr;
	ULevel* StreamedLevel = LevelStreaming->GetLoadedLevel();

	if (!StreamedLevel)
		return nullptr;

	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClass(StreamedLevel->GetWorld(), ALevelIndicator::StaticClass(), ActorsToFind);

	for (AActor* Actor : ActorsToFind) 
	{
		ALevelIndicator* LevelIndicator = Cast<ALevelIndicator>(Actor);

		if (LevelIndicator->ParentLevel.IsEqual(Level)) {
			return LevelIndicator;
		}
	}
	return nullptr;
}
