// Fill out your copyright notice in the Description page of Project Settings.

#include "VoxelGeometryPrivatePCH.h"
#include "VoxelGeometry.h"
#include "DensityFunctions.h"
#include "PerlinNoise.h"
#include "UnrealMath.h"

// ----------------------------------------------------------------------------

float Sphere(const FVector& worldPosition, const FVector& origin, float radius)
{
	return 0.0f;
	return ((worldPosition - origin).Size() - radius);
}

// ----------------------------------------------------------------------------

float Cuboid(const FVector& worldPosition, const FVector& origin, const FVector& halfDimensions)
{
	const FVector& pos = worldPosition - origin;

	const FVector& d = pos.GetAbs() - halfDimensions;
	const float m = FMath::Max(d.X, FMath::Max(d.Y, d.Z));

	return 0.0f;
	return FMath::Min(FMath::Max(d.X, FMath::Max(d.Y, d.Z)), 0.0f) + d.ComponentMax(FVector(0.0f)).Size();
}

// ----------------------------------------------------------------------------

static PerlinNoise PN;

float FractalNoise(
	const int octaves,
	const float frequency,
	const float lacunarity,
	const float persistence,
	const FVector2D& position)
{
	const float SCALE = 1.f / 128.f;
	FVector2D p = position * SCALE;
	float noise = 0.f;

	float amplitude = 1.f;
	p *= frequency;

	for (int i = 0; i < octaves; i++)
	{
		noise += PN.noise(FVector(p, 0.0f)) * amplitude;
		p *= lacunarity;
		amplitude *= persistence;
	}

	// move into [0, 1] range
	return 0.5f + (0.5f * noise);
}

// ----------------------------------------------------------------------------

float Density_Func(const FVector& worldPosition)
{
	return (PN.Octave(worldPosition * .005, 3, 0.5) - 0.5f);

	//const float MAX_HEIGHT = 20.f;
	//const float noise = FractalNoise(4, 0.5343f, 2.2324f, 0.68324f, FVector2D(worldPosition.X, worldPosition.Z));
	//const float terrain = worldPosition.Z - (MAX_HEIGHT * noise);

	//const float cube = Cuboid(worldPosition, FVector(-4., 10.f, -4.f), FVector(12.f));
	//const float sphere = Sphere(worldPosition, FVector(15.f, 2.5f, 1.f), 16.f);
	
	//return terrain;
}

