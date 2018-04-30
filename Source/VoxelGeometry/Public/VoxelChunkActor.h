// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

#include "VoxelGeometryFunctionLibrary.h"
#include "Async.h"
#include "GameFramework/Actor.h"

#include "UFNNoiseGenerator.h"

#include "VoxelChunkActor.generated.h"


UCLASS()
class VOXELGEOMETRY_API AVoxelChunkActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVoxelChunkActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Generates Chunk Geometry (Dual Contouring)
	UFUNCTION(BlueprintCallable, Category = ChunkActor)
	void BuildChunkAsync(UUFNNoiseGenerator* NoiseGenerator, float Threshold = 1.f);

	//Generates Chunk Geometry (Marching Cubes)
	UFUNCTION(BlueprintCallable, Category = ChunkActor)
	void BuildChunkAsyncMarchingCubes(UUFNNoiseGenerator* NoiseGenerator, float Threshold = 1.f, int32 VoxelsPerSide = 30, int32 ChunkSize = 1000);

protected: 

	/** Is chunk currently building*/
	bool bChunkIsBuilding;

	/* Chunk Geometry */
	FChunkGeometry ChunkGeometry;

	/*Stores and render voxel geometry */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChunkActor, Transient)
	class UProceduralMeshComponent* ProceduralMeshComponent;

	/* Size of octree */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChunkActor)
	int32 OctreeSize;
};
