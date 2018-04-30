// Fill out your copyright notice in the Description page of Project Settings.

#include "VoxelGeometryPrivatePCH.h"
#include "VoxelGeometry.h"
#include "VoxelChunkActor.h"
#include "ProceduralMeshComponent.h"
#include "VoxelOctree.h"


// Sets default values
AVoxelChunkActor::AVoxelChunkActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));

	OctreeSize = 4;
}

// Called when the game starts or when spawned
void AVoxelChunkActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVoxelChunkActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

static FChunkGeometry TestGeo;

void AVoxelChunkActor::BuildChunkAsync(UUFNNoiseGenerator* NoiseGenerator, float Threshold)
{
	VertexBuffer VBuff;
	IndexBuffer IBuff;

	OctreeNode* OctreeRoot = BuildOctree(GetActorLocation(), OctreeSize, Threshold);
	if (!OctreeRoot)
	{
		return;
	}

	GenerateMeshFromOctree(OctreeRoot, VBuff, IBuff);

	TArray<FVector> Verts, Norms;
	for (int i = 0; i < VBuff.Num(); ++i)
	{
		Verts.Add(VBuff[i].xyz);
		Norms.Add(VBuff[i].normal);
	}

	if (IsValid(ProceduralMeshComponent))
	{
		ProceduralMeshComponent->ClearAllMeshSections();
		const TArray<FVector2D> UV0;
		const TArray<FColor> VertexColors;
		const TArray<FProcMeshTangent> Tangents;
		ProceduralMeshComponent->CreateMeshSection(0, Verts, IBuff, Norms, UV0, VertexColors, Tangents, true);
	}

	DestroyOctree(OctreeRoot);
}

void AVoxelChunkActor::BuildChunkAsyncMarchingCubes(UUFNNoiseGenerator* NoiseGenerator, float Threshold, int32 VoxelsPerSide, int32 ChunkSize)
{
	if (!IsValid(NoiseGenerator))
	{
		return;
	}

	TFunction<void()> Task =
		[&]
	{
		float FloatChunkSize = (float)ChunkSize;
		float VoxPerSide = (float)VoxelsPerSide;
		FVector V(0.01f, 0.01f, 0.01f);
		FVector ActorLoc = GetActorLocation();
		TestGeo = UVoxelGeometryFunctionLibrary::GenerateChunkGeometry(NoiseGenerator, V, ActorLoc, FloatChunkSize, VoxPerSide, 0.5f);
	};

	TFunction<void()> Callback =
		[&, this]
	{
		TFunction<void()> GameThreadCode =
			[&, this]
		{
			if (IsValid(this) && IsValid(this->ProceduralMeshComponent))
			{
				this->ChunkGeometry = TestGeo;

				TArray<FVector> Normals;
				const TArray<FVector2D> UV0;
				const TArray<FColor> VertexColors;
				const TArray<FProcMeshTangent> Tangents;

				this->ProceduralMeshComponent->CreateMeshSection(0, this->ChunkGeometry.Vertices, this->ChunkGeometry.Indices, Normals, UV0, VertexColors, Tangents, true);
				this->bChunkIsBuilding = false;
			}
		};

		FFunctionGraphTask::CreateAndDispatchWhenReady(GameThreadCode, TStatId(), nullptr, ENamedThreads::GameThread);
	};

	if (!bChunkIsBuilding)
	{
		bChunkIsBuilding = true;
		Async<void>(EAsyncExecution::Thread, Task, Callback);
	}
}
