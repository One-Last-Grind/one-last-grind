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

void UMyCustomPhysicsLibrary::SetLevelOffset(UObject* WorldObject, TSoftObjectPtr<UWorld> Level, FVector WorldOffset, bool WorldShift) {
	UObject* WorldContextObject = WorldObject->GetWorld();
	ULevelStreaming* LevelStreaming = UGameplayStatics::GetStreamingLevel(WorldContextObject, GetLevelName(Level));
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

void UMyCustomPhysicsLibrary::GetLevelOffset(UObject* WorldObject, TSoftObjectPtr<UWorld> CurrentLevel, TSoftObjectPtr<UWorld> NextLevel, ALevelIndicator*& CurrentLevelIndicator, ALevelIndicator*& NextLevelIndicator)
{
	UObject* WorldContextObject = WorldObject->GetWorld();

	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClass(WorldObject->GetWorld(), ALevelIndicator::StaticClass(), ActorsToFind);

	ALevelIndicator* LevelIndicator = nullptr;

	for (AActor* Actor : ActorsToFind) 
	{
		LevelIndicator = Cast<ALevelIndicator>(Actor);

		if (LevelIndicator->GetParentLevelName().IsEqual(GetLevelName(NextLevel)) && LevelIndicator->ActorHasTag("End")) {
			NextLevelIndicator = LevelIndicator;
		}

		if (LevelIndicator->GetParentLevelName().IsEqual(GetLevelName(CurrentLevel)) && LevelIndicator->ActorHasTag("Start")) {
			CurrentLevelIndicator = LevelIndicator;
		}
	}
	return;
}

const FName UMyCustomPhysicsLibrary::GetLevelName(TSoftObjectPtr<UWorld> Level)
{
	return FName(Level.GetAssetName());
}

