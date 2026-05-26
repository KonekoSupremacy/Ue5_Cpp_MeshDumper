// No Copyright!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cpp_EditorTool_MeshDumper.generated.h"

/**
 * Editor Tool Actor to bulk spawn static meshes from a directory into the world.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Mesh Dumper Tool"))
class AURA_API ACpp_EditorTool_MeshDumper : public AActor
{
	GENERATED_BODY()
	
public:	
	//================================================================================================================
	// FUNCTIONS
	//================================================================================================================
	ACpp_EditorTool_MeshDumper();

	// Spawns the meshes from target directory
	UFUNCTION(CallInEditor, Category = "Mesh Dumper")
	void SpawnMeshes();

	// Clears all spawned meshes
	UFUNCTION(CallInEditor, Category = "Mesh Dumper")
	void ClearSpawnedMeshes();

	//================================================================================================================
	// PROPERTIES & VARIABLES
	//================================================================================================================
	// Path to search for static meshes (e.g. /Game/Folder)
	UPROPERTY(EditAnywhere, Category = "Mesh Dumper", meta = (ContentDir))
	FDirectoryPath TargetDirectory = FDirectoryPath("/Game/");

	// Max meshes to place in each row
	UPROPERTY(EditAnywhere, Category = "Mesh Dumper", meta = (ClampMin = "1"))
	int32 MeshesPerRow = 10;

	// Grid spacing between spawned actors
	UPROPERTY(EditAnywhere, Category = "Mesh Dumper")
	FVector2D Spacing = FVector2D(200.0f, 200.0f);
	
	// Prefix to add before each spawned mesh
	UPROPERTY(EditAnywhere, Category = "Mesh Dumper")
	FString PrefixToAdd = "Dumped_";

	// Align the bottom of each mesh to the dumper's plane
	UPROPERTY(EditAnywhere, Category = "Mesh Dumper")
	bool bAlignToFloor = true;

	// Scale up small meshes so they reach at least MinDimensionSize
	UPROPERTY(EditAnywhere, Category = "Mesh Dumper")
	bool bScaleToMinimumSize = false;

	// Minimum size in at least one dimension
	UPROPERTY(EditAnywhere, Category = "Mesh Dumper", meta = (EditCondition = "bScaleToMinimumSize", ClampMin = "1.0"))
	float MinDimensionSize = 100.0f;

	// Tracked spawned actors in the viewport
	UPROPERTY(VisibleAnywhere, Transient, Category = "Mesh Dumper")
	TArray<AActor*> SpawnedActors;
	
};
