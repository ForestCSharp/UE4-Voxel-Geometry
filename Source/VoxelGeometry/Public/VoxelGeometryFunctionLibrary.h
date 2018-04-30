// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UFNNoiseGenerator.h"

#include "VoxelGeometryFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct FMarchingCubesCell
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	TArray<FVector> Positions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grid)
	TArray<float> Values;

	FMarchingCubesCell()
	{
		Positions.Reserve(8);
		Values.Reserve(8);
	}
};

USTRUCT(BlueprintType)
struct FChunkGeometry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = Chunk)
	TArray<FVector> Vertices;

	UPROPERTY(BlueprintReadOnly, Category = Chunk)
	TArray<FVector> Normals;

	UPROPERTY(BlueprintReadOnly, Category = Chunk)
	TArray<int32> Indices;
};

UCLASS(BlueprintType)
class VOXELGEOMETRY_API UNoiseGeneratorInterface : public UObject
{
	GENERATED_BODY()

public:

	virtual float GetNoise3D(FVector Position) { return 0.0f; }
};

/**
 * 
 */
UCLASS()
class VOXELGEOMETRY_API UVoxelGeometryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable, Category = "Voxel")
	static TArray<FVector> PolygoniseCell(UPARAM(ref) FMarchingCubesCell& Cell, UPARAM(ref)float& IsoLevel);

	UFUNCTION(BlueprintCallable, Category = "Voxel")
	static FVector VertexInterp(float IsoLevel, UPARAM(ref)FVector& Pos1, UPARAM(ref)FVector& Pos2, UPARAM(ref)float& Val1, UPARAM(ref)float& Val2);

	//Generates chunk at StartPos of (Length,Width, and Height) == ChunkSize based on n VoxelsPerSide and outputs OutGeometry
	UFUNCTION(BlueprintCallable, Category = "Voxel")
	static FChunkGeometry GenerateChunkGeometry(UUFNNoiseGenerator* NoiseGenerator, UPARAM(ref) FVector& StartPos, UPARAM(ref)FVector& SamplePos, UPARAM(ref) float& ChunkSize, UPARAM(ref) float& VoxelsPerSide, float IsoLevel);

};
