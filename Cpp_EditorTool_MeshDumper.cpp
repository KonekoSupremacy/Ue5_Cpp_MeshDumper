// No Copyright!

#include "Cpp_EditorTool_MeshDumper.h"
#include "Engine/StaticMeshActor.h"
#include "AssetRegistry/AssetRegistryModule.h"



ACpp_EditorTool_MeshDumper::ACpp_EditorTool_MeshDumper() {
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneRoot;
}

void ACpp_EditorTool_MeshDumper::SpawnMeshes() {
#if WITH_EDITOR
	UE_LOG(LogTemp, Log, 
		TEXT("MeshDumper Settings: TargetDirectory='%s', bAlignToFloor=%s, bScaleToMinimumSize=%s, MinDimensionSize=%.2f"),
		*TargetDirectory.Path,
		bAlignToFloor ? TEXT("True") : TEXT("False"),
		bScaleToMinimumSize ? TEXT("True") : TEXT("False"),
		MinDimensionSize);

	// Clean up user path input
	FString SanitizedPath = TargetDirectory.Path.TrimStartAndEnd();
	SanitizedPath.ReplaceInline(TEXT("\\"), TEXT("/"));
	// Strip All folder prefixes
	if (SanitizedPath.StartsWith(TEXT("/All/Game/"))) {
		SanitizedPath = TEXT("/Game/") + SanitizedPath.RightChop(10);
	}
	else if (SanitizedPath.StartsWith(TEXT("/All/Content/"))) {
		SanitizedPath = TEXT("/Game/") + SanitizedPath.RightChop(13);
	}
	else if (SanitizedPath.StartsWith(TEXT("/All/"))) {
		SanitizedPath = TEXT("/Game/") + SanitizedPath.RightChop(5);
	}
	SanitizedPath.ReplaceInline(TEXT("//"), TEXT("/"));

	// Remove trailing slashes
	while (SanitizedPath.EndsWith(TEXT("/"))) {
		SanitizedPath.LeftChopInline(1);
	}

	// Force /Game path prefix
	if (!SanitizedPath.StartsWith(TEXT("/Game"))) {
		if (SanitizedPath.StartsWith(TEXT("/"))) {
			SanitizedPath = TEXT("/Game") + SanitizedPath;
		}
		else {
			SanitizedPath = TEXT("/Game/") + SanitizedPath;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("MeshDumper: Scanning path '%s' (Original input: '%s')"), *SanitizedPath, *TargetDirectory.Path);

	ClearSpawnedMeshes();

	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FString> PathsToScan;
	PathsToScan.Add(SanitizedPath);
	AssetRegistry.ScanPathsSynchronous(PathsToScan);

	// Setup Asset Registry filter
	FARFilter Filter;
	Filter.PackagePaths.Add(*SanitizedPath);
	Filter.bRecursivePaths = true;
	Filter.ClassPaths.Add(UStaticMesh::StaticClass()->GetClassPathName());

	TArray<FAssetData> AssetList;
	AssetRegistry.GetAssets(Filter, AssetList);

	if (AssetList.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("MeshDumper: No Static Meshes found in directory '%s'"), *SanitizedPath);
		return;
	}

	UWorld* World = GetWorld();
	if (!World) {
		return;
	}

	int32 CurrentRow = 0;
	int32 CurrentCol = 0;

	for (const FAssetData& Asset : AssetList) {
		UStaticMesh* LoadedMesh = Cast<UStaticMesh>(Asset.GetAsset());
		if (!LoadedMesh) {
			continue;
		}

		FVector LocalOffset = FVector(CurrentCol * Spacing.X, CurrentRow * Spacing.Y, 0.0f);
		FVector SpawnLocation = GetActorTransform().TransformPosition(LocalOffset);

		const FBoxSphereBounds Bounds = LoadedMesh->GetBounds();
		float ScaleFactor = 1.0f;

		if (bScaleToMinimumSize) {
			const float MaxSize = FMath::Max3(Bounds.BoxExtent.X, Bounds.BoxExtent.Y, Bounds.BoxExtent.Z);
			if (MaxSize > 0.0f && MaxSize < MinDimensionSize) {
				ScaleFactor = MinDimensionSize / MaxSize;
			}
		}

		if (bAlignToFloor) {
			// Align base of mesh to floor (scale adjusted)
			const float LocalMinZ = Bounds.Origin.Z - Bounds.BoxExtent.Z;
			SpawnLocation.Z -= (LocalMinZ * ScaleFactor);
		}

		FRotator SpawnRotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AStaticMeshActor* SpawnedActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), SpawnLocation, 
			SpawnRotation, SpawnParams);
		if (SpawnedActor) {
			SpawnedActor->GetStaticMeshComponent()->SetStaticMesh(LoadedMesh);
			SpawnedActor->SetActorLabel(FString::Printf(TEXT("%s%s"), *PrefixToAdd, *LoadedMesh->GetName()));
			SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			if (ScaleFactor != 1.0f) {
				SpawnedActor->SetActorScale3D(FVector(ScaleFactor));
				UE_LOG(LogTemp, Log, TEXT("MeshDumper: Scaled '%s' by %.2fx (Max local axis: %.2f)"), 
					*LoadedMesh->GetName(), ScaleFactor, Bounds.BoxExtent.GetMax() * 2.0f);
			}
			SpawnedActors.Add(SpawnedActor);
		}

		CurrentCol++;
		if (CurrentCol >= MeshesPerRow) {
			CurrentCol = 0;
			CurrentRow++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("MeshDumper: Successfully spawned %d meshes!"), SpawnedActors.Num());
#endif
}

void ACpp_EditorTool_MeshDumper::ClearSpawnedMeshes() {
#if WITH_EDITOR
	// Destroy tracked actors
	for (AActor* Actor : SpawnedActors) {
		if (IsValid(Actor)) {
			Actor->Destroy();
		}
	}
	SpawnedActors.Empty();

	// Fallback to clear any attached children
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (AActor* Actor : AttachedActors) {
		if (IsValid(Actor) && Actor->IsA<AStaticMeshActor>()) {
			Actor->Destroy();
		}
	}
#endif
}